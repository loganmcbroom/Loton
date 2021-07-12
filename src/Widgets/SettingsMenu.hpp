#pragma once

#include <QPushButton>

class MainWindow;
class QLabel;
class QHBoxLayout;
class NumberSlider;

class SettingsMenu : public QWidget
{ Q_OBJECT
public:
	SettingsMenu( QWidget * parent = nullptr );

	void paintMenu( const QPalette & );

	void addButton( int section, const QString & label, QAction * action );
	void addSlider( int section, const QString & label, NumberSlider * slider );

signals:
	void done();

private:
	QLabel * header;
	std::vector<QHBoxLayout *> boxes;

	struct ColorButton : public QPushButton
		{
		ColorButton( QPalette::ColorRole role );
		void selectColor();
		const QPalette::ColorRole role;
		};
	std::vector<ColorButton *> colorButtons;

};

