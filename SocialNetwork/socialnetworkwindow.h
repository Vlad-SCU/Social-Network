#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H

#include <QMainWindow>
#include "network.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SocialNetworkWindow;
}
QT_END_NAMESPACE

class SocialNetworkWindow : public QMainWindow
{
    Q_OBJECT

public:
    SocialNetworkWindow(QWidget *parent = nullptr);
    ~SocialNetworkWindow();

    void loginClicked();
    void display();
    void friendClicked(int row, int column);
    void returnClicked();
    void suggestClicked(int row, int column);
    void addClicked();
    void searchClicked();
    void backClicked();
    void createClicked();
    void submitClicked();

private:
    Ui::SocialNetworkWindow *ui;
    Network network;
    User* curUser;
    User* loginUser;
    User* prevUser;
    std::vector<User*> allUsers;
};

#endif // SOCIALNETWORKWINDOW_H

