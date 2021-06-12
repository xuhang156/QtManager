#pragma once

#include <QDialog>

extern const QString kUserId;
extern const QString kPassword;
extern const QString kOrganization;

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog();

private:
    void refreshUI();
    void initLanguage();
    void savePassword();
    void setUserInfoFromSettings();
    QString getLoginErrorStr();
    void displayLoginErrorStr(QString errorMsg);
private slots:
    void checkUser(const QString& user);
    void on_login_pressed();
    void on_swatch_language(QAction*);

private:
    QString _curUserRights;
    Ui::LoginDialog* ui;
};
