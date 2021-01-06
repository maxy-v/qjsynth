#pragma once
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
#include "ScriptColorScheme.h"
#include "ScriptKeyword.h"

// Note: only small subset of ECMA-262 syntax is supported

class ScriptSyntaxHighlighter: public QSyntaxHighlighter
{
	Q_OBJECT
public:
	explicit ScriptSyntaxHighlighter(QTextDocument* a_parent = nullptr);

protected:
	void highlightBlock(const QString& a_text) override;

private:
	void initNumbers();
	void initStrings();
	void initKeywords();
	void initFunctions();
	void initPunctuators();
	void initSingleLineComments();
	void initMultilineComments();
	void initApiArguments();
	void initApiSoundProperties();

	void highlightPunctuators(const QString& a_text);
	void highlightByRules(const QString& a_rext);
	void highlightMultilineComments(const QString& a_text);


	struct HighlightingRule	{
		QTextCharFormat    format;
		QRegularExpression pattern;
	};
	QList<HighlightingRule> m_rules;

	struct {
		QRegularExpression start;
		QRegularExpression end;
		QTextCharFormat    format;
	} m_multiline_comment_rule;

	struct {
		QTextCharFormat format;
		const QStringList punctuators = {
			"{","}","(",")","[","]",
			".",";",",",
			"<",">","<=",">=","==","!=",
			"+","-","*","/","%",
			"++","--",
			"<<",">>",
			"&","|","^","!","~",
			"&&","||",
			"?",":",
			"=","+=","-=","*=","/*","%=",
		};
	} m_punctuator_rule;
};