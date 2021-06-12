import os, sys
from time import sleep
from unified_service.json_service import JsonService, start_server_and_wait
from unified_service.caller import HubCaller
from random import uniform

hub_address = '127.0.0.1:5307'


class Vision3dSimu(JsonService):
    service_type = "vision3d"

    def __init__(self):
        # 1. 模拟poses
        self.poses = [[-0.4, -0.6, 0.2],
                      [-0.3, -0.6, 0.2],
                      [-0.2, -0.6, 0.1],
                      [-0.1, -0.6, 0],
                      [0.0, -0.6, -0.2]]
        # 2. 模拟labels
        self.id = [1]
        self.labels = [0.132]
        self.type = ['circle']
        self.holePosition = [-0.4, -0.6, 0.2,-0.4, -0.6, 0.2]

    def findPoses(self, *_):
        sleep(uniform(0, 2)) # 4. 模拟vision计算耗时
        poses = self.generate_poses()
        poses["function"] = "posesFound"
        HubCaller(hub_address).call("forward", {"type": "vision_watcher", "message": poses})
        return poses

    def setStepProperties(self, request_dict, _):
        print(request_dict)

    def generate_poses(self):
        objects = {"measuringPointId": self.id,
                   "diameter": self.labels,
                   "primitiveType": self.type,
                   "holeposition": self.holePosition
        }
        return objects


if __name__ == '__main__':
    start_server_and_wait(HubCaller(hub_address), Vision3dSimu(), "template")
        
