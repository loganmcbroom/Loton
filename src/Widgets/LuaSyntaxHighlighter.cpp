/***************************************************************************
 *   Copyright (C) 2007, 2008, 2009, 2011 by David Bitseff                 *
 *   bitsed@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "LuaSyntaxHighlighter.hpp"

LuaSyntaxHighlighter::LuaSyntaxHighlighter( QTextDocument * parent )
	: QSyntaxHighlighter( parent )
	{
	QColor monokai_yellow( "#E6DB74" );
	QColor monokai_orange( "#FD971F" );
	QColor monokai_red( "#F92672" );
	QColor monokai_magenta( "#FD5FF0" );
	QColor monokai_blue( "#66D9EF" );
	QColor monokai_green( "#A6E22E" );
	QColor monokai_cyan( "#A1EFE4" );
	QColor monokai_violet( "#AE81FF" );
	QColor monokai_gray( "#64645E" );
	QColor monokai_foreground( "#F8F8F2" );
	QColor monokai_background( "#272822" );
	QColor monokai_comments( "#75715E" );
	QColor monokai_emphasis( "#F8F8F0" );
	QColor monokai_line_number( "#8F908A" );
	QColor monokai_highlight( "#49483E" );
	QColor monokai_highlight_alt( "#3E3D31" );
	QColor monokai_highlight_line( "#3C3D37" );

	HighlightingRule rule;

	constantFormat.setForeground( monokai_green );
	rule.pattern = QRegExp("[-+]?(?:(?:\\d+\\.\\d+)|(?:\\.\\d+)|(?:\\d+\\.?))");
	rule.format = constantFormat;
	highlightingRules.append(rule);

	classFormat.setForeground( monokai_magenta );
	rule.pattern = QRegExp("\\b[A-Za-z][A-Za-z0-9_]*\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);

	functionFormat.setForeground( monokai_blue );
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+ *(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);

	keywordFormat.setForeground( monokai_red );
	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns
			<< "\\bdo\\b" << "\\bend\\b" << "\\bfor\\b" << "\\bwhile\\b"
			<< "\\bif\\b" << "\\bthen\\b" << "\\belse\\b" << "\\belseif\\b"
			<< "\\brepeat\\b" << "\\buntil\\b" << "\\bfunction\\b" << "\\blocal\\b"
			<< "\\breturn\\b" << "\\bin\\b" << "\\bbreak\\b" << "\\bnot\\b"
			<< "\\bnil\\b" << "\\band\\b" << "\\bor\\b" << "\\btrue\\b"
			<< "\\bfalse\\b";
	foreach (QString pattern, keywordPatterns)
	{
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

	quotationFormat.setForeground( monokai_orange );
	rule.pattern = QRegExp("\"[^\"]*\"");
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	rule.pattern = QRegExp("'[^']*'");
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	singleLineCommentFormat.setForeground( monokai_comments );
	singleLineCommentFormat.setFontItalic(true);
	rule.pattern = QRegExp("--[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);
}

void LuaSyntaxHighlighter::highlightBlock(const QString &text)
	{
	foreach (HighlightingRule rule, highlightingRules)
	{
		QRegExp expression(rule.pattern);
		int index = text.indexOf(expression);
		while (index >= 0)
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = text.indexOf(expression, index + length);
		}
	}
	}
