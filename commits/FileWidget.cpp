#include "FileWidget.h"

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

FileWidget::FileWidget(const QIcon &icon, const QString &text, QWidget *parent)
   : QFrame(parent)
   , mIcon(icon)
   , mButton(new QPushButton(mIcon, ""))
   , mText(new QLabel(text))
{
   const auto itemLayout = new QHBoxLayout(this);
   itemLayout->setContentsMargins(QMargins());
   mButton->setStyleSheet("max-width: 10px; min-width: 10px; max-height: 15px; min-height: 15px;");

   itemLayout->addWidget(mButton);
   itemLayout->addWidget(mText);

   if (!icon.isNull())
      connect(mButton, &QPushButton::clicked, this, [this]() { emit clicked(); });
}

FileWidget::FileWidget(const QString &icon, const QString &text, QWidget *parent)
   : FileWidget(QIcon(icon), text, parent)
{
}

QString FileWidget::text() const
{
   return mText->text();
}

void FileWidget::setText(const QString &text)
{
   mText->setText(text);
}
