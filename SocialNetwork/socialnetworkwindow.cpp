#include "socialnetworkwindow.h"
#include "ui_socialnetworkwindow.h"
#include "user.h"
#include "network.h"
#include "post.h"

using namespace std;

SocialNetworkWindow::SocialNetworkWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkWindow)
{
    ui->setupUi(this);

    network.readUsers("users.txt");
    network.readPosts("posts.txt");

    ui->friendsTable->hide();
    ui->profileLabel->hide();
    ui->postsLabel->hide();
    ui->returnButton->hide();
    ui->suggestTable->hide();
    ui->addButton->hide();
    ui->mutualTable->hide();
    ui->searchName->hide();
    ui->searchButton->hide();
    ui->backButton->hide();
    ui->enterBirth->hide();
    ui->enterZip->hide();
    ui->yearLabel->hide();
    ui->zipLabel->hide();
    ui->submitButton->hide();
    ui->friendsLabel->hide();
    ui->suggestLabel->hide();
    ui->postsLabel2->hide();

    connect(ui->loginButton, &QPushButton::clicked, this, &SocialNetworkWindow::loginClicked);
    connect(ui->friendsTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::friendClicked);
    connect(ui->returnButton, &QPushButton::clicked, this, &SocialNetworkWindow::returnClicked);
    connect(ui->suggestTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::suggestClicked);
    connect(ui->addButton, &QPushButton::clicked, this, &SocialNetworkWindow::addClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &SocialNetworkWindow::searchClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &SocialNetworkWindow::backClicked);
    connect(ui->createButton, &QPushButton::clicked, this, &SocialNetworkWindow::createClicked);
    connect(ui->submitButton, &QPushButton::clicked, this, &SocialNetworkWindow::submitClicked);
}

void SocialNetworkWindow::display() {
    int score;
    vector<int> friendsSuggestions = network.suggestFriends(curUser->getId(), score);
    ui->suggestTable->setRowCount(friendsSuggestions.size());
    ui->suggestTable->setColumnCount(1);

    int row1 = 0;

    for (auto friends : friendsSuggestions) {
        User* temp = network.getUser(friends);
        QString userName = QString::fromStdString(temp->getName());

        QTableWidgetItem* item = new QTableWidgetItem(userName);
        ui->suggestTable->setItem(row1, 0, item);
        row1++;
    }

    set<int> mutuals = network.mutualFriends(loginUser, curUser);
    ui->mutualTable->setRowCount(mutuals.size());
    ui->mutualTable->setColumnCount(1);
    int row = 0;

    for (auto friends : mutuals) {
        User* temp = network.getUser(friends);
        QString userName = QString::fromStdString(temp->getName());

        QTableWidgetItem* item = new QTableWidgetItem(userName);
        ui->mutualTable->setItem(row, 0, item);
        row++;
    }

    if (curUser->getId() == loginUser->getId()) {
        ui->profileLabel->setText("\t\tMy Profile");
        string recentPosts = network.getPostsString(curUser->getId(), 5, false);
        ui->postsLabel->setText(QString::fromStdString(recentPosts));
        ui->returnButton->hide();
        ui->suggestTable->show();
        ui->addButton->hide();
        ui->mutualTable->hide();
        ui->backButton->hide();
        allUsers.clear();
        ui->suggestLabel->setText("Friend Suggestions");
    } else {
        string name = curUser->getName();
        QString qName = QString::fromStdString(name);
        ui->profileLabel->setText("\t        " + qName + "'s Profile");
        string recentPosts = network.getPostsString(curUser->getId(), 5, true);
        ui->postsLabel->setText(QString::fromStdString(recentPosts));
        ui->returnButton->show();
        ui->suggestTable->hide();
        ui->addButton->show();
        ui->mutualTable->show();
        ui->backButton->show();
        ui->suggestLabel->setText("Mutual Friends");
    }

    ui->profileLabel->show();
    ui->postsLabel->show();

    set<int> friends = curUser->getFriends();
    ui->friendsTable->setRowCount(friends.size());
    ui->friendsTable->setColumnCount(1);

    int row2 = 0;

    for (auto friends : friends) {
        User* temp = network.getUser(friends);
        QString s = QString::fromStdString(temp->getName());

        QTableWidgetItem* item = new QTableWidgetItem(s);
        ui->friendsTable->setItem(row2, 0, item);
        row2++;
    }

    ui->friendsTable->show();
    ui->searchName->show();
    ui->searchButton->show();
    ui->friendsLabel->show();
    ui->suggestLabel->show();
    ui->postsLabel2->show();
}

void SocialNetworkWindow::loginClicked() {
    QString cellUser = ui->name->toPlainText();
    string userName = cellUser.toStdString();

    int id = network.getId(userName);
    loginUser = network.getUser(id);
    curUser = network.getUser(id);
    network.writeUsers("users.txt");

    if (id == -1) {
        ui->namePrompt->setText("Invalid name. Enter your name:");
    } else {
        ui->loginButton->hide();
        ui->name->hide();
        ui->namePrompt->hide();
        ui->createLabel->hide();
        ui->createButton->hide();
        display();
    }
}

void SocialNetworkWindow::friendClicked(int row, int column) {
    QTableWidgetItem* temp = ui->friendsTable->item(row, column);
    QString cellUser = temp->data(Qt::DisplayRole).toString();
    string userName = cellUser.toStdString();

    int id = network.getId(userName);
    allUsers.push_back(curUser);
    curUser = network.getUser(id);
    display();
}

void SocialNetworkWindow::returnClicked() {
    curUser = loginUser;
    display();
}

void SocialNetworkWindow::suggestClicked(int row, int column) {
    QTableWidgetItem* temp = ui->suggestTable->item(row, column);
    QString cellUser = temp->data(Qt::DisplayRole).toString();
    string userName = cellUser.toStdString();

    network.addConnection(loginUser->getName(), userName);
    network.writeUsers("users.txt");
    display();
}

void SocialNetworkWindow::addClicked() {
    network.addConnection(loginUser->getName(), curUser->getName());
    network.writeUsers("users.txt");
    display();
}

void SocialNetworkWindow::searchClicked() {
    QString user = ui->searchName->toPlainText();
    string userName = user.toStdString();

    int id = network.getId(userName);
    if (id == -1) {
        ui->searchButton->setText("Search");
    } else {
        allUsers.push_back(curUser);
        curUser = network.getUser(id);
        ui->searchName->clear();
        display();
    }

}

void SocialNetworkWindow::backClicked() {
    int n = allUsers.size();
    curUser = allUsers[n-1];
    allUsers.pop_back();
    display();
}

void SocialNetworkWindow::createClicked() {
    ui->loginButton->hide();
    ui->createLabel->hide();
    ui->createButton->hide();
    ui->enterBirth->show();
    ui->enterZip->show();
    ui->yearLabel->show();
    ui->zipLabel->show();
    ui->submitButton->show();
}

void SocialNetworkWindow::submitClicked() {
    QString temp1 = ui->name->toPlainText();
    string userName = temp1.toStdString();

    QString temp2 = ui->enterBirth->toPlainText();
    string userBirth = temp2.toStdString();
    int year = stoi(userBirth);

    QString temp3 = ui->enterZip->toPlainText();
    string userZip = temp3.toStdString();
    int zip = stoi(userZip);

    int id = network.numUsers();
    set<int> friends;

    User* newU = new User(id, userName, year, zip, friends);
    network.addUser(newU);

    ui->enterBirth->hide();
    ui->enterZip->hide();
    ui->yearLabel->hide();
    ui->zipLabel->hide();
    ui->submitButton->hide();
    ui->name->hide();
    ui->namePrompt->hide();

    loginClicked();
}

SocialNetworkWindow::~SocialNetworkWindow()
{
    delete ui;
}
