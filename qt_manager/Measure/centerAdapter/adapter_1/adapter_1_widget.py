
import logging
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.Qt import pyqtSlot
from interface.adapter import AdapterWidget
from util.message_box import warning_box
from util.translations import _translate

class Adapter_1Widget(AdapterWidget):
    def __init__(self):
        super().__init__()
        self.init_widget()
        self.model_type_list.addItems([])
        self.vision_name_list.addItems(['datumPosesAndPoses123Calculation'])

    @pyqtSlot()
    def on_set_model_clicked(self):
        model_type = self.model_type_list.currentText()
        type_start_index = model_type.find('_') + 1
        model_type = model_type[type_start_index:]
        if not model_type:
            warning_box(self, text=_translate("Form", "Empty model type!"))
            return
        try:
            self.adapter.vision_project_name = self.vision_name_list.currentText()
            self.adapter.set_step(model_type)
            self.adapter.msg_signal.emit(logging.INFO, _translate("Form", "Set model successfully!"))
        except Exception as e:
            logging.exception('Exception when set step: {}'.format(e))
            warning_box(self, text=_translate("Form", "Set model failed! Exception: {}".format(e)))

    def init_widget(self):
        self.gridLayout = QtWidgets.QGridLayout(self)
        self.gridLayout.setObjectName("gridLayout")
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.label = QtWidgets.QLabel(self)
        self.label.setAlignment(QtCore.Qt.AlignCenter)
        self.label.setObjectName("label")
        self.horizontalLayout.addWidget(self.label)
        self.model_type_list = QtWidgets.QComboBox(self)
        self.model_type_list.setObjectName("model_type_list")
        self.horizontalLayout.addWidget(self.model_type_list)
        self.label_1 = QtWidgets.QLabel(self)
        self.label_1.setAlignment(QtCore.Qt.AlignCenter)
        self.label_1.setObjectName("label_1")
        self.horizontalLayout.addWidget(self.label_1)
        self.vision_name_list = QtWidgets.QComboBox(self)
        self.vision_name_list.setObjectName("vision_name_list")
        self.horizontalLayout.addWidget(self.vision_name_list)
        self.set_model = QtWidgets.QPushButton(self)
        self.set_model.setObjectName("set_model")
        self.horizontalLayout.addWidget(self.set_model)
        self.gridLayout.addLayout(self.horizontalLayout, 0, 0, 1, 1)

        self.retranslateUi(self)
        QtCore.QMetaObject.connectSlotsByName(self)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "Form"))
        self.label.setText(_translate("Form", "Model Type"))
        self.label_1.setText(_translate("Form", "Vision Name"))
        self.set_model.setText(_translate("Form", "Set Model"))
