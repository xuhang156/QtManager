#include <QAction>
#include "util_gui.h"
#include "SQLCommand.h"
#include "LoginDialog.h"
#include "ui_LoginDialog.h"

const QString kUserTableName = QStringLiteral("userInfo");
const QString kUserId = QStringLiteral("userId");
const QString kRememberPassword = QStringLiteral("rememberPassword");
const QString kPassword = QStringLiteral("password");
const QStringList kRights = {QObject::tr("Administrator"), QObject::tr("Operator")};
const QString kUserRights = QStringLiteral("userRights");
const QString kOrganization = QStringLiteral("encrypt/encrypt");
const QHash<QString, QString> kLanguage = {{QObject::tr("Chinese"), QStringLiteral("zh_CN")},
                                           {QObject::tr("Japanese"), QStringLiteral("ja_JP")},
                                           {QObject::tr("English"), QStringLiteral("en_US")}};

QString getUserRights(QString userId, QString password = "", bool needPassword = false)
{
    SQLCommand sql(kUserTableName);
    QHash<QString, QString> userInfo;
    userInfo[kUserId] = userId;
    if (needPassword)
        userInfo[kPassword] = password;
    auto result = sql.select({kUserRights}, userInfo);
    while (result->next()) {
        return result->value(0).toString();
    }
    return "";
}

void LoginDialog::savePassword()
{
    mmind::mmindSettings().setValue(kRememberPassword, ui->rememberPassword->isChecked());
    mmind::mmindSettings().setValue(kUserId, ui->userId->text());
    mmind::mmindSettings().setValue(kPassword, ui->password->text());
}

void LoginDialog::setUserInfoFromSettings()
{
    ui->userId->setText(mmind::mmindSettings().value(kUserId).toString());
    ui->rememberPassword->setChecked(mmind::mmindSettings().value(kRememberPassword).toBool());
    if (ui->rememberPassword->isChecked())
        ui->password->setText(mmind::mmindSettings().value(kPassword).toString());
    checkUser(ui->userId->text());
}

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent), ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    initLanguage();
    setUserInfoFromSettings();
    displayLoginErrorStr("");
    connect(ui->userId, &QLineEdit::textChanged, this, &LoginDialog::checkUser);
}

LoginDialog::~LoginDialog() { delete ui; }

void LoginDialog::initLanguage()
{
    for (auto& iter : kLanguage.keys()) {
        QAction* action = new QAction(QObject::tr(iter.toStdString().c_str()));
        action->setObjectName(iter);
        ui->toolButton->addAction(action);
    }
    connect(ui->toolButton, &QToolButton::pressed, this, [=]() { ui->toolButton->showMenu(); });
    connect(ui->toolButton, &QToolButton::triggered, this, &LoginDialog::on_swatch_language);
}

void LoginDialog::on_login_pressed()
{
    savePassword();
    _curUserRights = getUserRights(ui->userId->text(), ui->password->text(), true);
    if (!_curUserRights.isEmpty()) {
        accept();
        displayLoginErrorStr("");
    } else
        displayLoginErrorStr(getLoginErrorStr());
}

void LoginDialog::on_swatch_language(QAction* action)
{
    auto findLanage = kLanguage.find(action->objectName());
    if (findLanage != kLanguage.end()) {
        mmind::TranslatorInstaller::instance().changeLanguage(findLanage.value());
        refreshUI();
    }
}

void LoginDialog::checkUser(const QString& user)
{
    const QString RightsStr = getUserRights(user);
    if (!RightsStr.isEmpty()) {
        ui->userType->setVisible(true);
        ui->userType->setText(QObject::tr(RightsStr.toStdString().c_str()));
    } else
        ui->userType->setVisible(false);
}

QString LoginDialog::getLoginErrorStr()
{
    if (!ui->userType->isVisible())
        return QString(QObject::tr("No UserID: %1")).arg(ui->userId->text());
    return QString(QObject::tr("Password Error"));
}

void LoginDialog::displayLoginErrorStr(QString errorMsg)
{
    errorMsg.isEmpty() ? ui->error->setVisible(false) : ui->error->setVisible(true);
    ui->error->setText(errorMsg);
}

void LoginDialog::refreshUI()
{
    ui->retranslateUi(this);
    setUserInfoFromSettings();
}
