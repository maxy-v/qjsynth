#include "ScriptSyntaxHighlighter.h"

ScriptSyntaxHighlighter::ScriptSyntaxHighlighter(QTextDocument* a_parent)
	: QSyntaxHighlighter(a_parent)
{
	initNumbers();
	initStrings();
	initKeywords();
	initFunctions();
	initPunctuators();
	initApiArguments();
	initApiSoundProperties();
	initSingleLineComments();
	initMultilineComments();
}

void ScriptSyntaxHighlighter::highlightBlock(const QString& a_text)
{
	highlightPunctuators(a_text);
	highlightByRules(a_text);
	highlightMultilineComments(a_text);
}

void ScriptSyntaxHighlighter::initNumbers()
{
	HighlightingRule rule;
	rule.format.setForeground( QColor(ScriptColorScheme::Number) );

	rule.pattern = QRegularExpression(QStringLiteral("\\d+(\\.\\d+)?")); // Integer/float
	m_rules << rule;
	rule.pattern = QRegularExpression(QStringLiteral("0[xX]{1}(\\d|[abcdefABCDEF])+")); // hex
	m_rules << rule;
	rule.pattern = QRegularExpression(QStringLiteral("0[oO]{1}(\\d|[abcdefABCDEF])+")); // oct
	m_rules << rule;
	rule.pattern = QRegularExpression(QStringLiteral("0[bB]{1}(\\d|[abcdefABCDEF])+")); // bin
	m_rules << rule;
}

void ScriptSyntaxHighlighter::initStrings()
{
	HighlightingRule rule;
	rule.format.setForeground( QColor(ScriptColorScheme::String) );
	rule.format.setFontItalic(true);
		
	rule.pattern = QRegularExpression(QStringLiteral("\".*\"")); // "string"
	m_rules << rule;
	rule.pattern = QRegularExpression(QStringLiteral("'.*'")); // 'string'
	m_rules << rule;
}

void ScriptSyntaxHighlighter::initKeywords()
{
	HighlightingRule rule;
	rule.format.setForeground( QColor(ScriptColorScheme::Keyword) );
	rule.format.setFontWeight( QFont::Bold );
	rule.pattern = QRegularExpression(QStringLiteral(
		"\\b("
			"break|"
			"case|continue|"
			"do|default|"
			"else|"
			"false|for|function|"
			"if|"
			"null|"
			"return|"
			"switch|"
			"true|"
			"var|void|"
			"while|"
		")\\b"));
	m_rules << rule;
}

void ScriptSyntaxHighlighter::initFunctions()
{
	HighlightingRule rule;
	rule.format.setForeground( QColor(ScriptColorScheme::Function) );
	rule.format.setFontWeight( QFont::Bold );
	rule.pattern = QRegularExpression(QStringLiteral(
		"\\b("
			"parseInt|parseFloat|isNaN|"
			"toString|"
			"(Math(\\.("
				"abs|sin|cos|min|max|pow|random|round|sqrt"
			")?)?)"
		")\\b"));
	m_rules << rule;
}

void ScriptSyntaxHighlighter::initPunctuators()
{
	m_punctuator_rule.format.setForeground( QColor(ScriptColorScheme::Punctuators) );
	m_punctuator_rule.format.setFontWeight( QFont::Bold );
}

void ScriptSyntaxHighlighter::initSingleLineComments()
{
	HighlightingRule rule;
	rule.format.setForeground( QColor(ScriptColorScheme::Comment) );
	rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
	m_rules << rule;
}

void ScriptSyntaxHighlighter::initMultilineComments()
{
	m_multiline_comment_rule.format.setForeground( QColor(ScriptColorScheme::Comment) );
	m_multiline_comment_rule.start = QRegularExpression(QStringLiteral("/\\*"));
	m_multiline_comment_rule.end   = QRegularExpression(QStringLiteral("\\*/"));
}

void ScriptSyntaxHighlighter::initApiArguments()
{
	HighlightingRule rule;
	rule.format.setForeground( QColor(ScriptColorScheme::ApiArg) );
	rule.format.setFontWeight( QFont::Bold );
	rule.pattern = QRegularExpression(QStringLiteral(
		"\\b"
			SCRIPT_KEYWORD_ARG "(\\.("
				    SCRIPT_KEYWORD_ARG_FRAME
				"|" SCRIPT_KEYWORD_ARG_FRAMES
			")?)?"
		"\\b"));
	m_rules << rule;
}

void ScriptSyntaxHighlighter::initApiSoundProperties()
{
	HighlightingRule rule;
	rule.format.setForeground( QColor(ScriptColorScheme::ApiSnd) );
	rule.format.setFontWeight( QFont::Bold );
	rule.pattern = QRegularExpression(QStringLiteral(
		"\\b"
			SCRIPT_KEYWORD_SND "(\\.("
				// Properties
				    SCRIPT_KEYWORD_SND_TYPE        
				"|" SCRIPT_KEYWORD_SND_VOLUME_FROM
				"|" SCRIPT_KEYWORD_SND_VOLUME_TO
				"|" SCRIPT_KEYWORD_SND_PITCH
				"|" SCRIPT_KEYWORD_SND_DUTY
				// Type constants
				"|" SCRIPT_KEYWORD_SND_SIN
				"|" SCRIPT_KEYWORD_SND_SQUARE
				"|" SCRIPT_KEYWORD_SND_TRIANGLE
				"|" SCRIPT_KEYWORD_SND_SAW
			")?)?"
		"\\b"));
	m_rules << rule;
}

void ScriptSyntaxHighlighter::highlightPunctuators(const QString& a_text)
{
	for (const QString& punctuator: m_punctuator_rule.punctuators)
	{
		for (int i = 0; i < a_text.size();)
		{
			int position = a_text.indexOf(punctuator, i);
			if (position == -1)
				break;
			setFormat(position, punctuator.size(), m_punctuator_rule.format);
			i = position + punctuator.size();
		}
	}
}

void ScriptSyntaxHighlighter::highlightByRules(const QString& a_text)
{
	for (const HighlightingRule& rule : m_rules)
	{
		QRegularExpressionMatchIterator match_iterator = rule.pattern.globalMatch(a_text);
		while (match_iterator.hasNext())
		{
			QRegularExpressionMatch match = match_iterator.next();
			setFormat(match.capturedStart(), match.capturedLength(), rule.format);
		}
	}
}

void ScriptSyntaxHighlighter::highlightMultilineComments(const QString& a_text)
{
	setCurrentBlockState(0);
	int start_index = 0;
	if (previousBlockState() != 1)
		start_index = a_text.indexOf(m_multiline_comment_rule.start);
	
	while (start_index >= 0)
	{
		QRegularExpressionMatch match = m_multiline_comment_rule.end.match(a_text, start_index);
		int end_index = match.capturedStart();
		int length = 0;
		if (end_index == -1)
		{
			setCurrentBlockState(1);
			length = a_text.length() - start_index;
		}
		else
			length = end_index - start_index + match.capturedLength();
		setFormat(start_index, length, m_multiline_comment_rule.format);
		start_index = a_text.indexOf(m_multiline_comment_rule.start, start_index + length);
	}
}
