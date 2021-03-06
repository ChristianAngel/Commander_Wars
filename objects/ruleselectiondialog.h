#ifndef RULESELECTIONDIALOG_H
#define RULESELECTIONDIALOG_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/ruleselection.h"
#include "objects/panel.h"

class RuleSelectionDialog;
typedef oxygine::intrusive_ptr<RuleSelectionDialog> spRuleSelectionDialog;

class RuleSelectionDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit RuleSelectionDialog();
    virtual ~RuleSelectionDialog() = default;
signals:
    void sigRulesChanged();
    void sigShowLoadRules();
    void sigShowSaveRules();
public slots:
    void showLoadRules();
    void showSaveRules();
    void loadRules(QString filename);
    void saveRules(QString filename);
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_pButtonLoadRules;
    oxygine::spButton m_pButtonSaveRules;
    spRuleSelection m_pRuleSelection;
    spPanel m_pPanel;
};

#endif // RULESELECTIONDIALOG_H
