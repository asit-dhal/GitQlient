#include "ConfigWidget.h"

#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QSettings>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QStyle>

ConfigWidget::ConfigWidget(QWidget *parent)
   : QFrame(parent)
   , mOpenRepo(new QPushButton(tr("Open existing repo")))
   , mCloneRepo(new QPushButton(tr("Clone new repo")))
   , mInitRepo(new QPushButton(tr("Init new repo")))
{
   mOpenRepo->setObjectName("bigButton");
   mCloneRepo->setObjectName("bigButton");
   mInitRepo->setObjectName("bigButton");

   const auto line = new QFrame();
   line->setObjectName("separator");

   // Adding buttons to open or init repos
   const auto repoSubtitle = new QLabel(tr("Repository options"));
   repoSubtitle->setObjectName("subtitle");

   const auto repoOptionsFrame = new QFrame();
   const auto repoOptionsLayout = new QVBoxLayout(repoOptionsFrame);
   repoOptionsLayout->setSpacing(20);
   repoOptionsLayout->setContentsMargins(QMargins());
   repoOptionsLayout->addWidget(repoSubtitle);
   repoOptionsLayout->addWidget(mOpenRepo);
   repoOptionsLayout->addWidget(mCloneRepo);
   repoOptionsLayout->addWidget(mInitRepo);
   repoOptionsLayout->addWidget(line);

   QSettings s;
   const auto mostUsedRepos = s.value("lastUsedRepos", QStringList()).toStringList();
   for (const auto &repo : mostUsedRepos)
   {
      const auto usedRepo = new QPushButton(repo);
      usedRepo->setToolTip(repo);
      repoOptionsLayout->addWidget(usedRepo);
      connect(usedRepo, &QPushButton::clicked, this,
              [this]() { emit signalOpenRepo(dynamic_cast<QPushButton *>(sender())->toolTip()); });
   }

   repoOptionsLayout->addStretch();

   const auto usedSubtitle = new QLabel(tr("Configuration"));
   usedSubtitle->setObjectName("subtitle");

   const auto configFrame = new QFrame();
   const auto configLayout = new QVBoxLayout(configFrame);
   configLayout->setContentsMargins(QMargins());
   configLayout->setSpacing(20);
   configLayout->addWidget(usedSubtitle);
   // configLayout->addWidget(createConfigWidget());
   configLayout->addStretch();

   const auto widgetsLayout = new QHBoxLayout();
   widgetsLayout->setContentsMargins(QMargins());
   widgetsLayout->setSpacing(150);
   widgetsLayout->addWidget(repoOptionsFrame);
   widgetsLayout->addWidget(configFrame);

   const auto title = new QLabel(tr("Welcome to GitQlient"));
   title->setObjectName("title");

   const auto lineTitle = new QFrame();
   lineTitle->setObjectName("separator");

   const auto centerLayout = new QVBoxLayout();
   centerLayout->setSpacing(20);
   centerLayout->setContentsMargins(QMargins());
   centerLayout->addWidget(title);
   centerLayout->addWidget(lineTitle);
   centerLayout->addLayout(widgetsLayout);
   centerLayout->addStretch();

   const auto layout = new QGridLayout(this);
   layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 0);
   layout->addLayout(centerLayout, 1, 1);
   layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 2, 2);

   connect(mOpenRepo, &QPushButton::clicked, this, &ConfigWidget::openRepo);
}

void ConfigWidget::openRepo()
{
   const QString dirName(QFileDialog::getExistingDirectory(this, "Choose the directory of a Git project"));

   if (!dirName.isEmpty())
   {
      QDir d(dirName);
      emit signalOpenRepo(d.absolutePath());
   }
}

QWidget *ConfigWidget::createConfigWidget()
{
   mBtnGroup = new QButtonGroup();
   mBtnGroup->addButton(new QPushButton(tr("General")));
   mBtnGroup->addButton(new QPushButton(tr("Git config")));
   mBtnGroup->addButton(new QPushButton(tr("Profiles")));

   const auto buttons = mBtnGroup->buttons();
   const auto buttonsLayout = new QVBoxLayout();
   buttonsLayout->setContentsMargins(QMargins());

   auto count = 0;
   for (auto btn : buttons)
   {
      buttonsLayout->addWidget(btn);

      if (count < buttons.count() - 1)
      {
         ++count;
         const auto line = new QFrame();
         line->setObjectName("separator2px");
         buttonsLayout->addWidget(line);
      }
   }

   buttonsLayout->addStretch();

   const auto testWidget = new QWidget();
   testWidget->setObjectName("testWidget");

   const auto stackedWidget = new QStackedWidget();
   stackedWidget->setMinimumHeight(300);
   stackedWidget->addWidget(testWidget);
   stackedWidget->addWidget(new QWidget());
   stackedWidget->addWidget(new QWidget());
   stackedWidget->setCurrentIndex(0);

   connect(mBtnGroup, qOverload<int>(&QButtonGroup::buttonClicked), this, [this, stackedWidget](int index) {
      const auto selectedBtn = mBtnGroup->button(index);
      const auto buttons = mBtnGroup->buttons();

      for (auto btn : buttons)
      {
         btn->setProperty("selected", selectedBtn == btn);
         btn->style()->unpolish(btn);
         btn->style()->polish(btn);
      }

      stackedWidget->setCurrentIndex(index);
   });

   const auto tabWidget = new QFrame();
   tabWidget->setObjectName("tabWidget");
   const auto layout = new QHBoxLayout(tabWidget);
   layout->setSpacing(0);
   layout->setContentsMargins(QMargins());
   layout->addLayout(buttonsLayout);
   layout->addWidget(stackedWidget);

   return tabWidget;
}

QWidget *ConfigWidget::createConfigPage()
{
   return nullptr;
}

QWidget *ConfigWidget::createGeneralPage()
{
   return nullptr;
}

QWidget *ConfigWidget::createProfilesPage()
{
   return nullptr;
}