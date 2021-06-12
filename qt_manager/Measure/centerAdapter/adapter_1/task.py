import uuid, time, json, sqlite3
from enum import IntFlag
import os
from .movementRules import getRobotMovementRules
from .jsonKey import *
from .config import *

dbPath = 'D:/measure.db'
class MeasureType(IntFlag):
    productType = 1
    trayType = 2

class Task:
    def __init__(self):
        self.sqlStrs = []
        self.taskId = ''
        self.productId = ''
        self.robotName = 'AR1440'

    def __init_task_statistics_table(self,curDate):
        con = sqlite3.connect(dbPath)
        rows = con.execute("SELECT * FROM taskStatistics where time='{0}'".format(curDate))
        if len(rows.fetchall()) == 0:
            self.sqlStrs.append("insert into taskStatistics values({0},'{1}','{2}','{3}')".format('null',curDate,0,0))

    def reset(self):
        self.sqlStrs = []

    def create_task(self, measureStr):
        self.taskId = str(uuid.uuid1())
        dateTime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
        date = dateTime.split(' ')[0]
        print(measureStr)
        measureDictData = json.loads(measureStr)
        # measureDictData = json.loads(json.dumps(measureStr))

        self.productId = measureDictData['productNumber']
        self.__init_task_statistics_table(date)
        self.sqlStrs.append("insert into taskInfo values('{0}','{1}','{2}','{3}','{4}','{5}','{6}','{7}','{8}','{9}','0',0,0);".format(self.taskId
                                                                                                          ,dateTime
                                                                                                          ,measureDictData['measureType']
                                                                                                          ,measureDictData['measuredescription']
                                                                                                          ,measureDictData['operator']
                                                                                                          ,measureDictData['trayNumber']
                                                                                                          ,measureDictData['productNumber']
                                                                                                          ,measureDictData['moduel']
                                                                                                          ,measureDictData['isFullTest']
                                                                                                          ,measureDictData['event']

                                                                                                          ))
        measureType = 'productsMeasureNumber'
        if measureDictData['measureType'] == MeasureType.trayType:
            measureType = 'trayMeasureNumber'
        self.sqlStrs.append("update taskStatistics set {0} = {0} +1 where time = '{1}'".format(measureType,date))

    def getProductId(self):
        return self.productId

    def get_task_id(self):
        return self.taskId

    def getRobotName(self):
        return self.robotName

    def writeTask(self):
        con = sqlite3.connect(dbPath)
        for sql in self.sqlStrs:
            con.execute(sql)
        con.commit()
        self.sqlStrs.clear()
        con.close()

    def __del__(self):
        self.con.close()

class StateMachine:
    def __init__(self):
        self._state = "over"
        self._stepIndex = -1
        self._steps = []

    def initState(self, states):
        self._steps = states

    def start(self):
        if len(self._steps) == 0 or self.isRunning():
            return False
        self._stepIndex = 0
        self._state = self._steps[self._stepIndex]

    def isRunning(self):
        return self._state != 'over'

    def curState(self):
        return self._state

    def updateState(self):
        if self._state == 'over':
            return False
        if len(self._steps) - 1 == self._stepIndex:
            self._state = 'over'
            self._stepIndex = -1
        else:
            self._stepIndex += 1
            self._state = self._steps[self._stepIndex]
        return True

class MeasureHub:
    def __init__(self):
        self.task = Task()
        self.visionResult = VisionResult()
        self.robot = RobotCommunication()

    def start(self, taskStr):
        if self.robot.endOfMeasurementTask():
            self.task.create_task(taskStr)
            self.visionResult.set_task_id(self.task.get_task_id())
            self.robot.initRobotMovementRules(self.task.getProductId(), self.task.getRobotName())
            print('start ok')
            return True
        return False

    def getRobotJointAngle(self):
        if self.robot.end_of_robot_movement() == False:
            return {}
        return self.robot.getRobotCurPosition()

    def end_of_robot_movement(self):
        return self.robot.end_of_robot_movement()

    def endOfMeasurementTask(self):
        return self.robot.endOfMeasurementTask()

    def getRobotNextPosition(self):
        return self.robot.getRobotNextPosition()

    def enable_robot_flag(self, whichRobot, position):
        self.robot.enable_robot_flag(whichRobot, position)

    def handleNext(self):
        self.robot.updateRobot()

    def reset(self):
        self.robot.reset()
        self.visionResult.clear()
        self.task.reset()

    def saveResult(self):
        if self.robot.endOfMeasurementTask():
            try:
                self.task.writeTask()
                self.visionResult.writeVisionResult()
                return True
            except Exception as e:
                self.reset()
                return False

class RobotCommunication:
    def __init__(self):
        self.jobState = StateMachine()
        self.movementRules = {"step1": {'right': 123, 'left': 355},'step2': {'right': 123, 'left': 355}}
        self.robotFlag = {'left': False, 'right': False}                    #一次测量时的标志
        self.robotPosition = {'left': {}, 'right': {}}                      #当前两个机器人移动完成后的位置

    def initRobotMovementRules(self,productId,robotName):
        if not self.movementRules:
            return False
        # self.jobState.initState(list(self.movementRules.keys()))
        self.jobState.initState(get_steps(productId,True))
        self.jobState.start()
        return True

    def enable_robot_flag(self, whichRobot, position):
        if whichRobot not in self.robotFlag:
            return False
        self.robotFlag[whichRobot] = True
        self.robotPosition[whichRobot] = position
        return True

    def reset(self):
        self.resetRobotFlag()

    def resetRobotFlag(self):
        for key in self.robotFlag.keys():
            self.robotFlag[key] = False
            self.robotPosition[key] = {}

    def endOfMeasurementTask(self):
        return self.jobState.isRunning() == False

    def end_of_robot_movement(self):
        return False not in self.robotFlag.values()

    def updateRobot(self):
        if self.end_of_robot_movement():
            self.jobState.updateState()
            self.resetRobotFlag()
            return True
        return False

    def getRobotCurPosition(self):
        if self.end_of_robot_movement():
            return self.robotPosition.get('left')
        return {}

    def getRobotNextPosition(self):
        return self.movementRules.get(self.jobState.curState())


class VisionResult:
    def __init__(self):
        self.projectName = 'FrSus_018D'
        self.taskId = ''
        self.results = []

    def set_task_id(self, task):
        self.taskId = task

    def clear(self):
        self.results.clear()

    def writeVisionResult(self):
        con = sqlite3.connect(dbPath)
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
            self.sqlStrs = "insert into {0} values('{1}','{2}','{3}',{4},{5},{6},{7},{8},'{9}','{10}')".format(self.projectName,
                                                                                                             self.taskId,
                                                                                                             result[MEASURING_POINT_ID][0],
                                                                                                             result[PRIMITIVE_TYPE][0],
                                                                                                             longAxis,
                                                                                                             shortAxis,
                                                                                                             result[HOLE_POSITION][0][0],
                                                                                                             result[HOLE_POSITION][0][1],
                                                                                                             result[HOLE_POSITION][0][2],
                                                                                                             result[HOLE_POSITION][0][2],
                                                                                                             2)
            con.execute(self.sqlStrs)
        con.commit()
        con.close()

    def addNodeResult(self,resultJsonStr):
        self.results.append(resultJsonStr)