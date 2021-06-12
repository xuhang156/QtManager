import uuid, time, json, sqlite3
from enum import IntFlag
import os
# from .movementRules import getRobotMovementRules
from .jsonKey import *
from .config import *

db_path = 'D:/measure.db'
tray_measure_type = 2
k_products_measure_number = "productsMeasureNumber"
k_tray_measure_number = "trayMeasureNumber"

class TaskState(IntFlag):
    running = 1
    over = 2

class Task:
    def __init__(self):
        self.reset()
        self.robot_name = 'AR1440'

    def __init_task_statistics_table(self, cur_date):
        con = sqlite3.connect(db_path)
        rows = con.execute("select * from taskStatistics where time='{0}'".format(cur_date))
        if len(rows.fetchall()) == 0:
            self.sql_str.append("insert into taskStatistics values({0},'{1}','{2}','{3}')".format('null', cur_date, 0, 0))

    def reset(self):
        self.sql_str = []
        self.task_id = ''
        self.product_id = ''

    def create_task(self, task_json):
        self.task_id = str(uuid.uuid1())
        date_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
        date = date_time.split(' ')[0]
        measure_dict_data = json.loads(task_json)
        self.product_id = '{}_{}'.format(measure_dict_data['productNumber'], measure_dict_data['moduel'])
        self.__init_task_statistics_table(date)
        self.sql_str.append("insert into taskInfo values('{0}','{1}','{2}','{3}','{4}','{5}','{6}','{7}','{8}','{9}','0',0,0);".format(self.task_id
                                                                                                          ,date_time
                                                                                                          ,measure_dict_data['measureType']
                                                                                                          ,measure_dict_data['measuredescription']
                                                                                                          ,measure_dict_data['operator']
                                                                                                          ,measure_dict_data['trayNumber']
                                                                                                          ,measure_dict_data['productNumber']
                                                                                                          ,measure_dict_data['moduel']
                                                                                                          ,measure_dict_data['isFullTest']
                                                                                                          ,measure_dict_data['event']
                                                                                                          ))
        measure_type = k_products_measure_number
        if measure_dict_data['measureType'] == tray_measure_type:
            measure_type = k_tray_measure_number
        self.sql_str.append("update taskStatistics set {0} = {0} +1 where time = '{1}'".format(measure_type, date))

    def get_product_id(self):
        return self.product_id

    def get_task_id(self):
        return self.task_id

    def get_robot_name(self):
        return self.robot_name

    def write_task(self):
        con = sqlite3.connect(db_path)
        for sql in self.sql_str:
            con.execute(sql)
        con.commit()
        self.sql_str.clear()
        con.close()

    def __del__(self):
        self.con.close()

class MeasureHub:
    def __init__(self):
        self.task = Task()
        self.vision_result = VisionResult()
        self.robot = RobotCommunication()
        self.task_flag = TaskState.over

    def start(self, task_str):
        if self.end_of_measurement_task():
            self.task.create_task(task_str)
            self.vision_result.set_project_name(self.task.get_product_id())
            self.vision_result.set_task_id(self.task.get_task_id())
            self.task_flag = TaskState.running
            return True
        return False

    def end_of_robot_movement(self):
        return self.robot.end_of_robot_movement()

    def end_of_measurement_task(self):
        return self.task_flag == TaskState.over

    def set_end_of_measurement_task(self):
        self.task_flag = TaskState.over

    def enable_robot_flag(self, which_robot, position):
        return self.robot.enable_robot_flag(which_robot, position)

    def reset(self):
        self.robot.reset()
        self.vision_result.clear()
        self.task.reset()

    def save_result(self):
        try:
            self.task.write_task()
            self.vision_result.write_vision_result()
            return True
        except Exception as e:
            self.reset()
        return False

class RobotCommunication:
    def __init__(self):
        self.robot_flag = {'left': False, 'right': False}                    #一次测量时的标志
        self.robot_position = {'left': {}, 'right': {}}                      #当前两个机器人移动完成后的位置

    def enable_robot_flag(self, which_robot, position):
        if which_robot not in self.robot_flag or not position:
            return False
        self.robot_flag[which_robot] = True
        self.robot_position[which_robot] = position
        return True

    def reset(self):
        self.reset_robot_flag()

    def reset_robot_flag(self):
        for key in self.robot_flag.keys():
            self.robot_flag[key] = False
            self.robot_position[key] = {}

    def end_of_robot_movement(self):
        return False not in self.robot_flag.values()

    def get_robot_cur_position(self):
        if self.end_of_robot_movement():
            return self.robot_position.get('left')
        return {}

class VisionResult:
    def __init__(self):
        self.project_name = ''  #'FrSus_018D'
        self.task_id = ''
        self.results = []

    def set_task_id(self, task):
        self.task_id = task

    def set_project_name(self, name):
        self.project_name = name

    def clear(self):
        self.results.clear()

    def write_vision_result(self):
        con = sqlite3.connect(db_path)
        for resultStr in self.results:
            result = json.loads(resultStr)
            longAxis = ''
            shortAxis = ''
            if result[PRIMITIVE_TYPE][0] == 'circle':
                longAxis = result[DIAMETER][0]
                shortAxis = result[DIAMETER][0]
            else:
                longAxis = result[LONGAXIS][0]
                shortAxis = result[SHORTAXIS][0]
            self.sql_str = "insert into {0} values('{1}','{2}','{3}',{4},{5},{6},{7},{8},'{9}','{10}')".format(self.project_name,
                                                                                                             self.task_id,
                                                                                                             result[MEASURING_POINT_ID][0],
                                                                                                             result[PRIMITIVE_TYPE][0],
                                                                                                             longAxis,
                                                                                                             shortAxis,
                                                                                                             result[HOLE_POSITION][0][0],
                                                                                                             result[HOLE_POSITION][0][1],
                                                                                                             result[HOLE_POSITION][0][2],
                                                                                                             result[HOLE_POSITION][0][2],
                                                                                                             2)
            con.execute(self.sql_str)
        con.commit()
        con.close()

    def add_node_result(self, result_json):
        self.results.append(result_json)