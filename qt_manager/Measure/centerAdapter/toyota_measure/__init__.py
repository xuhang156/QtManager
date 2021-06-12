
import sys
import os
from adapter.toyota_measure.toyota_measure_adapter import Toyota_Measure_Adapter

_adater_path = os.path.abspath(os.path.join(os.path.dirname(__file__)))
if _adater_path not in sys.path:
    sys.path.append(_adater_path)

adapter_dict = {os.path.basename(_adater_path): Toyota_Measure_Adapter}

