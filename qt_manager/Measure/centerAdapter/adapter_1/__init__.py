
import sys
import os
from adapter.adapter_1.adapter_1_adapter import Adapter_1Adapter
from adapter.adapter_1.adapter_1_widget import Adapter_1Widget

_adater_path = os.path.abspath(os.path.join(os.path.dirname(__file__)))
if _adater_path not in sys.path:
    sys.path.append(_adater_path)

adapter_dict = {os.path.basename(_adater_path): Adapter_1Adapter}

