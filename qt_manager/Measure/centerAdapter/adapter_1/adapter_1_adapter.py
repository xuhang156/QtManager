
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


model_dict = {}
model_properties_dict = {}
tcp_client_dict = {'192.168.3.240': 'task_computer', '192.168.3.145': 'robotLeft'}
not_use_result = ['datum1_radius','datum2_radius','datum3_radius','datumPosesAndPoses123Calculation']

class Adapter_1Adapter(TcpServerAdapter):
    vision_project_name = ""
    is_force_real_run = True
    service_name = "adapter_1 Adapter"

    def __init__(self, address):
        super().__init__(address)
        self.robot_service = None
        self.set_recv_size(1024)
        self.measure_hub = MeasureHub()
        self.testUpTaskOnce = False
        try:
            self._register_service()
        except Exception as e:
            self.msg_signal.emit(logging.ERROR, str(e))
            return
        
    def set_vision_project_from_robot_point(self, point_id):
        # FengtianMeasureAdapter.vision_project_name = '{}{}'.format(product_name_to_vision_prefix.get(self.measure_hub.task.getProductId()), point_id)
        Adapter_1Adapter.vision_project_name = get_visions(self.measure_hub.task.productId,True,point_id)

    def run_next(self,which_Step):
        if self.measure_hub.robot.jobState.isRunning() == False:
            self.msg_signal.emit(logging.ERROR, 'No measurement task')
            return
        #获取位姿，根据当前的工程和步骤
        self.set_vision_project_from_robot_point(self.measure_hub.robot.jobState.curState())
        self.measure_hub.enable_robot_flag('left',get_position(self.measure_hub.task.productId,Adapter_1Adapter.vision_project_name))
        self.measure_hub.enable_robot_flag('right',get_position(self.measure_hub.task.productId,Adapter_1Adapter.vision_project_name))
        if self.measure_hub.end_of_robot_movement() == True:
            self.robot_service.setL(self.measure_hub.robot.getRobotCurPosition())
            sleep(0.1)  # wait for a cycle of getting in Vision
            try:
                self.measure_hub.handleNext()
                str  = self.find_vision_pose().decode()
                # str = '{"diameter":[15.189419747370636],"holePosition":[[1292.3696005577012,-379.5069917387999,931.485860530205]],"measuringPointId":["孔6-2下"],"noCloudInRoi":false,"primitiveType":["circle"]}'
                if self.vision_project_name in not_use_result:
                    return
                self.measure_hub.visionResult.addNodeResult(str)
            except Exception as e:
                logging.exception(e),
                self.msg_signal.emit(logging.ERROR, 'Error: {}'.format(e))
        if self.measure_hub.endOfMeasurementTask() == True:
            if self.measure_hub.saveResult() == True:
                self.msg_signal.emit(logging.INFO, 'Save success')
            else:
                self.msg_signal.emit(logging.ERROR, 'Save failed')

    # 接收指令 _create_received_section
    def handle_command(self, cmds):
        strCmd = cmds.decode()
        machine_type = 'task_computer'
        if machine_type == 'task_computer':
            if self.testUpTaskOnce == True and strCmd == 'next':
                self.run_next(strCmd)
                return
            if self.measure_hub.start(str(strCmd)) == False:
                self.msg_signal.emit(logging.ERROR, 'Measurement task started')
            else:
                self.msg_signal.emit(logging.INFO, 'Start measurement task')
                self.testUpTaskOnce = True
            return
        elif machine_type == 'robotLeft' or machine_type == 'robotRight':
            self.run_next(strCmd)
            return

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
        # for name, properties in model_properties_dict.items():
        #     values = {}
        #     for p in properties:
        #         values[p] = model_dict[model_type][p.lower()]
        #     self.set_step_property(step_message(name, values))

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
    service_name = "FANUC_M710IC_45M"

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