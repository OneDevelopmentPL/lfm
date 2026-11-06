#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <cstddef>
#include <fstream>
#include <QCloseEvent>
#include <QSettings>
#include <vector>
#include <algorithm>

const int windowWidth = 450;
const int windowHeight = 300;

// long long  howManyToAdd = 1;

// long long clicks = 0;
// long long money = 0;

bool isAutoSaveOn = false;
bool isAutoLoadOn = false;

class Game {
private:
    long long clicks = 0;
    long long money = 0;
    long long howManyToAdd = 1;
public:
    long long returnClicks() {return clicks;}
    long long returnMoney() {return money;}
    long long returnHowManyToAdd() {return howManyToAdd;}

    void addClick() {clicks += howManyToAdd;}

    bool exchange(long long clickCost, long long moneyReward) {
        if (clicks < clickCost) return false;
        clicks -= clickCost;
        money += moneyReward;
        return true;
    }

    bool buyClickUpgrade(long long moneyCost) {
        if (money < moneyCost) return false;
        money -= moneyCost;
        howManyToAdd++;
        return true;
    }

    long long sellAll() {
        long long earned = clicks / 10;
        clicks %= 10;
        money += earned;
        return earned;
    }

    void setState(long long c, long long m, long long h) {clicks = c; money = m; howManyToAdd = h;}

    bool spend(long long amount) {
        if (money < amount) return false;
        money -= amount;
        return true;
    }
};

Game game;

class achievementService {
private:
    std::vector<long long> achievementsList;
public:
    void addToAchievementsList(long long whatToAdd) {
        if(!hasAchievement(whatToAdd)) {
            achievementsList.push_back(whatToAdd);
        }
    }
    const std::vector<long long>& getFullAchievementsList() const {
        return achievementsList;
    }
    long long getAchievementsList(int index) const {
        return achievementsList[index];
    }
    bool hasAchievement(long long id) const {
        return std::find(
            achievementsList.begin(),
            achievementsList.end(),
            id
        ) != achievementsList.end();
    }
    void clearAchievements() {
        achievementsList.clear();
    }
};

achievementService achievements;

void refreshAchievementsLabel(QLabel *label) {
    QString text = "Osiągnięcia:\n";
    if (achievements.getFullAchievementsList().empty()) {
        text += "(brak)\n";
    } else {
        for (long long id : achievements.getFullAchievementsList()) {
            text += QString("- %1 kliknięć %2").arg(id).arg("\n");
        }
    }
    label->setText("Pobieranie osiągnięć...");
    QTimer::singleShot(500, [label, text](){
        label->setText(text);
    });
}

void showAchievement(long long id) {
    QWidget *achievementsWindow = new QWidget();
    QLabel *achievementLabel = new QLabel();
    QVBoxLayout *achievementLayout = new QVBoxLayout(achievementsWindow);
    achievementLabel->setText(
        QString("Brawo, zdobyłeś %1 kliknięć!").arg(id)
    );
    achievementLayout->addWidget(achievementLabel);
    achievementsWindow->setWindowTitle("Osiągnięcie");
    achievementsWindow->show();
    QTimer::singleShot(5000, [achievementsWindow](){
        achievementsWindow->close();
    });
}

void saveAsFile() {
    std::ofstream file("clicker_data.as756dfg97fd59jk8776v8s5df8r1");

    if (file.is_open()){
        file << game.returnClicks() << "\n";
        file << game.returnMoney() << "\n";
        file << game.returnHowManyToAdd() << "\n";

        file << achievements.getFullAchievementsList().size() << "\n";

        for (long long id : achievements.getFullAchievementsList()) {
            file << id << "\n";
        }
        file.close();
    }
}

void loadFile() {
    std::ifstream file("clicker_data.as756dfg97fd59jk8776v8s5df8r1");

    if(file.is_open()) {
        achievements.clearAchievements();
        long long clicks = game.returnClicks();
        long long money = game.returnMoney();
        long long howManyToAdd = game.returnHowManyToAdd();
        file >> clicks >> money >> howManyToAdd;
        game.setState(clicks, money, howManyToAdd);

        size_t count = 0;
        if (file >> count) {
            for (size_t i = 0; i < count; i++) {
                long long id;
                if (file >> id) {
                    achievements.addToAchievementsList(id);
                }
            }
        }
        file.close();
        
    } else {
        
    }
}

void checkAchievements() {
    const long long milestones[] = {50, 100, 200, 500, 1000};

    for (long long id: milestones) {
        if (game.returnClicks() >= id && !achievements.hasAchievement(id)) {
            achievements.addToAchievementsList(id);
            showAchievement(id);
        }
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc,argv);


    QSettings settings("Clicker", "Clicker");

    isAutoLoadOn = settings.value("autoLoad", false).toBool();
    isAutoSaveOn = settings.value("autoSave", false).toBool();

    // Main window

    QWidget *window = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout(window);

    QHBoxLayout *btnLayout = new QHBoxLayout();

    QPushButton *centerBtn = new QPushButton("Kliknij");
    QPushButton *settingsBtn = new QPushButton("Ustawienia");
    QPushButton *shopBtn = new QPushButton("Sklep");
    centerBtn->setFixedSize(150, 50);

    QLabel *label = new QLabel("Kliknięcia: 0");

    // Settings window

    QWidget *settingsWindow = new QWidget();
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsWindow);

    QLabel *settingsLabel = new QLabel();
    QPushButton *settingsOkBtn = new QPushButton("OK");
    QPushButton *loadFromFileBtn = new QPushButton("Załaduj z pliku");
    QPushButton *saveAsFileBtn = new QPushButton("Zapisz do pliku");
    QPushButton *toggleAutoLoad = new QPushButton();
    QPushButton *toggleAutoSave = new QPushButton();


    // Main window

    btnLayout->addStretch();
    btnLayout->addWidget(label);
    btnLayout->addWidget(centerBtn);
    btnLayout->addStretch();

    layout->addStretch();
    layout->addLayout(btnLayout);
    layout->addStretch();

    layout->addWidget(settingsBtn);
    layout->addWidget(shopBtn);

    window->resize(windowWidth, windowHeight);
    window->setWindowTitle("Clicker");

    // Settings window

    // settingsLayout->addWidget(settingsLabel);
    settingsLayout->addWidget(saveAsFileBtn);
    settingsLayout->addWidget(loadFromFileBtn);
    settingsLayout->addSpacing(20);
    settingsLayout->addWidget(toggleAutoSave);
    settingsLayout->addWidget(toggleAutoLoad);
    settingsLayout->addSpacing(20);
    settingsLayout->addWidget(settingsLabel);
    settingsLayout->addSpacing(20);
    settingsLayout->addWidget(settingsOkBtn);

    window->setAttribute(Qt::WA_DeleteOnClose);

    settingsWindow->setWindowTitle("Ustawienia");

    // Shop

    QWidget *shopWindow = new QWidget();
    QVBoxLayout *shopLayout = new QVBoxLayout(shopWindow);
    QLabel *moneyLabel = new QLabel(QString("Stan konta: %1").arg(game.returnMoney()));
    QPushButton *exchangeBtn = new QPushButton("Zamien 10 kliknięć na 1 monetę");
    QPushButton *exchange50Btn = new QPushButton("Zamien 50 kliknięć na 5 monet");
    QPushButton *plusTwoForMoneyBtn = new QPushButton("Zamien 10 monet na dodatkowe +1 przy kliknięciu");
    QPushButton *closeShopBtn = new QPushButton("Zamknij");
    QPushButton *autoClickerBtn = new QPushButton("AutoClicker na minutę za 35 monet");
    QTimer *autoclickerTimer = new QTimer(window);
    QPushButton *sellAllBtn = new QPushButton("Sprzedaj wszystko");

    autoclickerTimer->setInterval(250);


    shopLayout->addWidget(moneyLabel);
    shopLayout->addWidget(exchangeBtn);
    shopLayout->addWidget(exchange50Btn);
    shopLayout->addWidget(sellAllBtn);
    shopLayout->addSpacing(25);
    shopLayout->addWidget(plusTwoForMoneyBtn);
    shopLayout->addWidget(autoClickerBtn);

    shopLayout->addSpacing(25);
    shopLayout->addWidget(closeShopBtn);

    shopWindow->setWindowTitle("Sklep");


    QPushButton::connect(centerBtn, &QPushButton::clicked, [centerBtn, label](){
        game.addClick();
        label->setText(QString("Kliknięcia: %1").arg(game.returnClicks()));
        
        checkAchievements();
    });

    QPushButton::connect(settingsBtn, &QPushButton::clicked, [settingsWindow,settingsLabel](){
        settingsWindow->show();
        refreshAchievementsLabel(settingsLabel);
    });
    
    QPushButton::connect(settingsOkBtn, &QPushButton::clicked, [settingsWindow](){settingsWindow->close();});

    QPushButton::connect(saveAsFileBtn, &QPushButton::clicked, [label, settingsWindow](){
        saveAsFile();
        label->setText("Pomyslnie zapisano stan gry do pliku!");
        QTimer::singleShot(3000, settingsWindow, [label](){
            label->setText(QString("Kliknięcia: %1").arg(game.returnClicks()));
        });
    });

    QPushButton::connect(loadFromFileBtn, &QPushButton::clicked, [label, settingsWindow](){
        loadFile();
        label->setText(QString("Kliknięcia: %1").arg(game.returnClicks()));
    });

    QPushButton::connect(toggleAutoSave, &QPushButton::clicked, [toggleAutoSave, &settings](){
        isAutoSaveOn = !isAutoSaveOn;
        if(isAutoSaveOn) {
            toggleAutoSave->setText(QString("AutoZapis: ON"));
        } else {toggleAutoSave->setText(QString("AutoZapis: OFF"));}

        settings.setValue("autoSave", isAutoSaveOn);
    });

    QPushButton::connect(toggleAutoLoad, &QPushButton::clicked, [toggleAutoLoad, &settings](){
        isAutoLoadOn = !isAutoLoadOn;
        if(isAutoLoadOn) {
            toggleAutoLoad->setText(QString("AutoOdczyt: ON"));
        } else {toggleAutoLoad->setText(QString("AutoOdczyt: OFF"));}

        settings.setValue("autoLoad", isAutoLoadOn);
    });

    QPushButton::connect(shopBtn, &QPushButton::clicked, [shopWindow](){
        shopWindow->show();
    });

    QPushButton::connect(exchangeBtn, &QPushButton::clicked, [moneyLabel, label, shopWindow](){
        if(game.exchange(10, 1)) {
            moneyLabel->setText(QString("Stan konta: %1").arg(game.returnMoney()));
            label->setText(QString("Kliknięcia: %1").arg(game.returnClicks()));
        } else {
            moneyLabel->setText("Masz za mało kliknięć, aby to kupić!");
            QTimer::singleShot(1000, shopWindow, [moneyLabel](){
                moneyLabel->setText(QString("Stan konta: %1").arg(game.returnMoney()));
            });
        }
    });

    QPushButton::connect(exchange50Btn, &QPushButton::clicked, [moneyLabel, label, shopWindow](){
        if(game.exchange(50, 5)) {
            moneyLabel->setText(QString("Stan konta: %1").arg(game.returnMoney()));
            label->setText(QString("Kliknięcia: %1").arg(game.returnClicks()));
        } else {
            moneyLabel->setText("Masz za mało kliknięć, aby to kupić!");
            QTimer::singleShot(1000, shopWindow, [moneyLabel](){
                moneyLabel->setText(QString("Stan konta: %1").arg(game.returnMoney()));
            });
        }
    });

    QPushButton::connect(plusTwoForMoneyBtn, &QPushButton::clicked, [moneyLabel,shopWindow](){
        if(game.buyClickUpgrade(10)) {
            moneyLabel->setText(QString("Stan konta: %1").arg(game.returnMoney()));
        } else {
            moneyLabel->setText("Masz za mało monet, aby to kupić!");
            QTimer::singleShot(1000, shopWindow, [moneyLabel](){
                moneyLabel->setText(QString("Stan konta: %1").arg(game.returnMoney()));
            });
        }
    });

    QPushButton::connect(closeShopBtn, &QPushButton::clicked, [shopWindow](){
        shopWindow->close();
    });

    QObject::connect(autoclickerTimer, &QTimer::timeout, [label](){
        game.addClick();

        label->setText("Kliknięcia: " + QString::number(game.returnClicks()));

        checkAchievements();
    });

    QObject::connect(autoClickerBtn, &QPushButton::clicked, [autoclickerTimer,autoClickerBtn,moneyLabel, shopWindow](){
        if (autoclickerTimer->isActive()) {
            autoclickerTimer->stop();
            autoClickerBtn->setText("AutoClicker na minutę za 35 monet");
        } 
        else if (game.spend(35)) {
            moneyLabel->setText(QString("Stan konta: %1").arg(game.returnMoney()));

            autoclickerTimer->start();

            autoClickerBtn->setText("AutoClicker: ON");

            QTimer::singleShot(60000, autoclickerTimer, [&, autoclickerTimer](){
                autoclickerTimer->stop();
                autoClickerBtn->setText("AutoClicker na minutę za 35 monet");
            });
        }
        else {
            moneyLabel->setText("Masz za mało monet!");

            QTimer::singleShot(1500, shopWindow, [moneyLabel](){
                moneyLabel->setText(QString("Stan konta: %1").arg(game.returnMoney()));
            });
        }
    });
    QPushButton::connect(sellAllBtn, &QPushButton::clicked, [moneyLabel,label](){
        long long earned = game.sellAll();
        if (earned > 0) {
            moneyLabel->setText(QString("Stan konta: %1").arg(game.returnMoney()));
            label->setText(QString("Kliknięcia: %1").arg(game.returnClicks()));
        }
    });

    window->show();
    // On open
    if (isAutoLoadOn) {
        loadFile();
        label->setText(QString("Kliknięcia: %1").arg(game.returnClicks()));
        toggleAutoLoad->setText("AutoOdczyt: ON");
    } else {toggleAutoLoad->setText("AutoOdczyt: OFF");}
    // On close
    QObject::connect(&app, &QApplication::aboutToQuit, [](){
        if(isAutoSaveOn) {
            saveAsFile();
        } else {}
    });

    // while (clicks != 50) {
    //     showAchievement("Brawo! Zdobyłeś 50 kliknięć!");
    // }
    if(isAutoSaveOn) {
        toggleAutoSave->setText("AutoZapis: ON");
    } else {toggleAutoSave->setText("AutoZapis: OFF");}
    return QApplication::exec();
}
