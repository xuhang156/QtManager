
import json
import logging
import math
import sys
from struct import pack, unpack
from time import sleep
import threading
import os

import uuid
from .task import MeasureHub
from .config import *

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..")))
from transforms3d import quaternions, affines, euler
from interface.adapter import TcpServerAdapter, TcpClientAdapter
from interface.services import RobotService, VisionResultSelectedAtService, register_service
from util.transforms import object2tcp


next_step_command_len = 3
star_task_command_len = 2
end_task_command_len = 2
not_use_result = ['datum1_radius', 'datum2_radius', 'datum3_radius', 'datumPosesAndPoses123Calculation']

class Toyota_Measure_Adapter(TcpServerAdapter):
    vision_project_name = ""
    service_name = "Toyota_Measure Adapter"

    def __init__(self, address):
        super().__init__(address)
        self.robot_service = None
        self.set_recv_size(1024)
        self.measure_hub = MeasureHub()
        try:
            self._register_service()
        except Exception as e:
            self.msg_signal.emit(logging.ERROR, str(e))
            return

    # 接收指令 _create_received_section
    # t,json             ->任务
    # r,step,whichRobot  ->机器人到位，当前测量节点
    # e,0                ->结束任务
    def handle_command(self, cmds):
        commands = cmds.decode().split('&')
        cmd_len = len(commands)

        if commands[0] == "t" and cmd_len == star_task_command_len:
            self.create_task(commands[1])
        if commands[0] == "e" and cmd_len == end_task_command_len:
            self.end_task()
        if commands[0] == "r" and cmd_len == next_step_command_len:
            self.run_next(commands[1], commands[2])
        return

    def create_task(self, task_json):
        if not self.measure_hub.start(task_json):
            self.msg_signal.emit(logging.ERROR, '测量任务已启动')
        else:
            self.msg_signal.emit(logging.INFO, '开启测量任务')

    def end_task(self):
        if not self.measure_hub.end_of_measurement_task():
            self.measure_hub.set_end_of_measurement_task()
            if self.measure_hub.save_result():
                self.msg_signal.emit(logging.INFO, '保存成功')
                return
        self.msg_signal.emit(logging.ERROR, '保存失败')

    def run_next(self, point_name, which_robot):
        if self.measure_hub.end_of_measurement_task():
            self.msg_signal.emit(logging.ERROR, '未启动测量任务')
            return
        self.set_vision_project_name(point_name)
        if not self._set_robot_position(which_robot):
            return

        if self.measure_hub.end_of_robot_movement():
            try:
                self.robot_service.setL(self.measure_hub.robot.get_robot_cur_position())
                sleep(0.1)
                vision_result = self.find_vision_pose().decode()
                if self.vision_project_name in not_use_result:
                    return
                self.measure_hub.vision_result.add_node_result(vision_result)
                self.msg_signal.emit(logging.INFO, '节点测量结束: {}'.format(point_name))
            except Exception as e:
                logging.exception(e),
                self.msg_signal.emit(logging.ERROR,  'Error: {}'.format(e))

    def set_vision_project_name(self, point_id):
        Toyota_Measure_Adapter.vision_project_name = point_id
        # Toyota_Measure_Adapter.vision_project_name = get_visions(self.measure_hub.task.productId, True, point_id)

    def _set_robot_position(self, which_robot):
        flag_left = self.measure_hub.enable_robot_flag('left', get_position(self.measure_hub.task.get_product_id(),
                                                                       Toyota_Measure_Adapter.vision_project_name))
        flag_right = self.measure_hub.enable_robot_flag('right', get_position(self.measure_hub.task.get_product_id(),
                                                                 Toyota_Measure_Adapter.vision_project_name))
        if not flag_left or not flag_right:
            self.msg_signal.emit(logging.ERROR, '未能正确设置机器人位姿')
        return flag_right and flag_left

    # 检查vision结果
    def check_vision_result(self, vision_result, at=None):
        poses = vision_result.get('poses')
        labels = vision_result.get('labels')
        if not poses or not poses[0]:
            self.msg_signal.emit(logging.ERROR, '无视觉点')
            self.send(('1002' + '' + '').encode())
            return

        self.send(self.pack_pose(poses, labels).encode())
        self.msg_signal.emit(logging.INFO, '发送点位成功')

    # 打包pose _pack_pose_section
    def pack_pose(self, poses, labels, at=None):
        
        pack_count = len(poses)
        msg_body = ''
        for i in range(pack_count):
            pose = poses[i]
            object2tcp(pose)
            t = [p * 1000 for p in pose[:3]]
            r = [math.degrees(p) for p in euler.quat2euler(pose[3:], 'sxyz')]
            p = t + r
            self.msg_signal.emit(logging.INFO, '发送pose: {}'.format(p))
            msg_body += ('{:.3f},' * (len(p) - 1) + '{:.3f}').format(*p)
            
            if i != (pack_count - 1):
                msg_body += ','
        return '{},{},'.format(1100, pack_count) + msg_body + ''

    # 注册机器人服务和Vision Watcher服务 _register_service_section
    def _register_service(self):
        if self.robot_service:
            return
        
        self.robot_service = AdapterRobotService(self.extract_moves_from_viz, self.send_nocloud_from_viz)
        other_info = {'robot_type': self.robot_service.service_name}
        self.server, _ = register_service(self.hub_caller, self.robot_service, other_info)
        self.set_step_property({'type': 'Capture Images from Camera', 'values': {'robotName': '{}'.format(self.robot_service.service_name)}})

    def _unregister_service(self):
        if self.robot_service:
            self.hub_caller.unregister_service(self.robot_service.service_name)

    def close(self):
        super().close()
        self._unregister_service()

    def set_step(self, model_type):
        pass

    def extract_moves_from_viz(self, moves):
        pick_move_num = 0
        jps_moves = []
        tcp_pose_moves = []
        motion_types = []
        for i, move in enumerate(moves):
            jps_moves.append(move["target"])
            if move.get("tcp_pose"):
                tcp_pose_moves.append(move.get("tcp_pose"))
            motion_types.append(move["motion_type"])
            if move.get("is_visual_move"):
                pick_move_num = i + 1
        self.pack_moves(pick_move_num, jps_moves, tcp_pose_moves, motion_types)

    def send_nocloud_from_viz(self):
    
        self.msg_signal.emit(logging.ERROR, '点云无结果')
        self.send(('1003' + '' + '').encode())

    def pack_moves(self, pick_move_num, jps_moves, tcp_pose_moves, motion_types):
        place_move_num = len(jps_moves) - pick_move_num

        msg = '{},'.format(1100)
        for i, move in enumerate(jps_moves):
            msg += ('{:.3f},' * 5 + '{:.3f}').format(*move)
            msg += ','
        msg = msg[:-1]
        self.send(msg.encode())
        self.msg_signal.emit(logging.INFO, '发送点位成功')

class AdapterRobotService(RobotService):
    service_name = "AR440"

    def __init__(self, send_moves, send_nocloud):
        self._jps = [0] * 6
        self._pose = [0] * 3 + [1,0,0,0]
        self.send_moves = send_moves
        self.send_nocloud = send_nocloud
        self.move_targets = []
        self.lock = threading.Lock()

    # getJ 是给vision提供关节角, *此函数不可删除* _getJ_section
    def getJ(self, *_):
        return

    # getL 是给vision提供法兰姿态 *此函数不可删除* _getL_section
    def getL(self, *_):
        pose = {'tcp_pose': self._pose}
        return pose

    # 关节角单位必须转换为弧度 _setJ_section
    def setJ(self, jps):
        logging.info("setJ:{}".format(jps))

    # 法兰姿态必须转换为四元数形式；单位为米；如果从机器人发来的是欧拉角形式，需要转换为四元数 _setL_section
    def setL(self, pose):
        logging.info("setL:{}".format(pose))

        t = pose[:3]
        r = pose[3:]
        for i, j in enumerate(t):
            t[i] = float(j)
        for i, j in enumerate(r):
            r[i] = float(j)
        self._pose = t + list(r)

    # moveXs用于接收Viz发过来的点位信息
    def moveXs(self, params, _):
        logging.info("moveXs: {}".format(params))
        with self.lock:
            for move in params["moves"]:
                self.move_targets.append(move)
        return {"finished": True}

    def notify(self, request, _):
        msg = request["notify_message"]
        logging.info("{} notify message:{}".format(self.service_name, msg))
        if msg == "started":
            with self.lock:
                self.move_targets.clear()
        elif msg == "finished":
            with self.lock:
                targets = self.move_targets[:]
                self.move_targets.clear()
            self.send_moves(targets)
        elif msg == "noCloud":
            self.send_nocloud()