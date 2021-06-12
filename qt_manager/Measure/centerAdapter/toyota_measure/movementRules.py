# rulesJson = [
#     {'productId': '081D_FrSUS', 'robotName': 'AR1440', 'rules': {'step1': {'left': {'step1': 1}, 'right': {'step1': 1}}, 'step2': {'left': {'step2': 2}, 'right': {'step2': 2}}}},
#     {'productId': '081D_RrSUS', 'robotName': 'AR1440', 'rules': {'step1': {'left': {}, 'right': {}}, 'step2': {'left': {}, 'right': {}},'step3':{'left': {},'right': {}}}}
# ]
#
# def getRobotMovementRules(productId, robotName):
#     #从配置文件中直接获取
#     for ruls in rulesJson:
#         if productId == ruls.get('productId') and robotName == ruls.get('robotName'):
#             return ruls.get('rules')
#     return {}
#
