/*
*    PseuArknights
*
*    Copyright (C) 2023  Tyler Parret True
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published
*    by the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* @Authors
*    Tyler Parret True <mysteryworldgod@outlook.com><https://github.com/OwlHowlinMornSky>
*/
#include "gamecore.h"

#include <locale>

float universialVolumn = 100.0f;

inline void setStr(sf::Text * t_t, const char* ch) { // 将从文本(UTF-8)读取到的字符串应用于sf::Text
	t_t->setString(sf::String::fromUtf8(ch, ch + strlen(ch)));
	return;
}

char ui_version[32];
unsigned int characterCnt = 0;
sf::Font ui_font, sp_font, li_font;

bool title(sf::RenderWindow& window); // 标题
void mainMenu(sf::RenderWindow& window); // 主页
void mapMenu(sf::RenderWindow& window, OHMS::AudioCtrl& ac); // 作战
bool BattleCtrl(sf::RenderWindow& window, OHMS::AudioCtrl& ac); // 进入战斗
void BattleMain(sf::RenderWindow& window, std::string LVname); // 战斗
void ListOfProducers(sf::RenderWindow& window); // 制作人员名单
void gameOver(sf::RenderWindow& window, int rank, sf::Image battleOver, OHMS::AudioCtrl* ac); // 结算
sf::Image MOHUAAA(sf::Image im, int radius); // 线性模糊
bool tipPop(sf::RenderWindow& window);

sf::Texture globalCloth_TX, rhodesCloth_TX;
sf::Vector2u globalCloth_TX_SIZE, rhodesCloth_TX_SIZE;
sf::Sprite globalCloth, rhodesCloth;

bool GOtoBattle, GOtoList;
std::string levelName;

int main() {
	std::locale::global(std::locale("en-us.utf8"));

	printf_s("本程序仅供个人研究学习使用。\n请自觉于下载后24小时内删除。\n");

	srand((unsigned int)time(0));

	OHMS::filescanf("assets/version.txt", "%s", ui_version, 32);
	OHMS::filescanf("assets/character/total.txt", "%d", &characterCnt); // Read Char Cnt

	sf::Image im;
	im.loadFromFile("assets/icon.png");
	im.flipVertically();
	im.flipHorizontally();
	sf::Vector2u iconsize = im.getSize();

	sf::RenderWindow window(sf::VideoMode(winWidth, winHight), "Archknights", sf::Style::Default ^ sf::Style::Resize); // Make Window  sf::Style::None
	window.setKeyRepeatEnabled(true);
	window.setIcon(iconsize.x, iconsize.y, im.getPixelsPtr());
	window.setFramerateLimit(60);

	OHMS::initLihuiShader();

	ui_font.loadFromFile("assets/ui/font/SourceHanSansSC-Regular.otf"); // Load Font 
	sp_font.loadFromFile("assets/ui/font/SourceHanSerifSC-Heavy.otf");
	li_font.loadFromFile("assets/ui/font/Trueno.otf");

	globalCloth_TX.loadFromFile("assets/ui/texture/cloth.png");
	globalCloth_TX_SIZE = globalCloth_TX.getSize();
	globalCloth.setTexture(globalCloth_TX);
	globalCloth.setColor(sf::Color(0, 0, 0, 255));
	globalCloth.setScale(winWidth * 1.0f / globalCloth_TX_SIZE.x, winHight * 1.0f / globalCloth_TX_SIZE.y);
	rhodesCloth_TX.loadFromFile("assets/ui/texture/bk_rhodes.png");
	rhodesCloth_TX_SIZE = rhodesCloth_TX.getSize();
	rhodesCloth.setTexture(rhodesCloth_TX);
	rhodesCloth.setColor(sf::Color(0, 0, 0, 0));
	rhodesCloth.setScale(winWidth * 1.0f / rhodesCloth_TX_SIZE.x, winHight * 1.0f / rhodesCloth_TX_SIZE.y);


	title(window);
menu:
	GOtoBattle = false;
	GOtoList = false;
	if (window.isOpen()) {
		mainMenu(window);
	}
	if (GOtoBattle) {
		BattleMain(window, levelName);
		goto menu;
	}
	if (GOtoList) {
		ListOfProducers(window);
		goto menu;
	}
	return 0;
}

void ListOfProducers(sf::RenderWindow &window) {
	sf::Clock clock;
	float timer;
	sf::Event event;

	OHMS::AudioCtrl audioVoid(1, 1); // Load Main Menu Audio 
	audioVoid.setBGM(0, "assets/audio/music/", "shop");
	audioVoid.setSound(0, "assets/audio/sounds/", "ui_back");
	audioVoid.setVolumn(universialVolumn);

	sf::Image img;
	img.create(100, 60, sf::Color(0, 0, 0));

	sf::Texture btn;
	btn.loadFromImage(img);

	OHMS::GSROOT graphicRoot_void_0;

	OHMS::GSbuttonArray* graphicBtn_void_btn_0 = new OHMS::GSbuttonArray;
	OHMS::GStext* graphicBtn_void_list = new OHMS::GStext;

	graphicRoot_void_0.setSubGS(0, graphicBtn_void_btn_0);
	graphicRoot_void_0.setSubGS(1, graphicBtn_void_list);

	graphicRoot_void_0.setPosAbs({ winWidth / 2.0f, winHight / 2.0f });

	graphicBtn_void_btn_0->initialize(&window);

	graphicBtn_void_btn_0->push_back(&window, btn);
	graphicBtn_void_btn_0->setPosition(0, 5.0f, 5.0f);
	graphicBtn_void_btn_0->setStyle(0, 200, 140);
	graphicBtn_void_btn_0->setText(0, ui_font, L"<", 30);
	graphicBtn_void_btn_0->setTextPosition(0, 42, 10);

	sf::Text* list = graphicBtn_void_list->getText();
	list->setString(L"***本项目仅供个人研究学习使用***");
	list->setFillColor(sf::Color::Black);
	list->setFont(ui_font);
	list->setCharacterSize(40);
	graphicBtn_void_list->setOrgRate({ 0.5f, 0.5f });

	graphicBtn_void_list->setPosToParent({ -100.0f, -100.0f });

	graphicBtn_void_btn_0->isActive = true;

	audioVoid.playBGM(0);

	clock.restart();
	while ((timer = clock.getElapsedTime().asSeconds()) < 0.2f) {
		while (window.pollEvent(event))
			;
		window.clear(sf::Color::White);
		graphicRoot_void_0.displayIn(window);
		globalCloth.setColor(sf::Color(0, 0, 0, (int)(255 * std::max((0.15f - timer) / 0.15f, 0.0f))));
		window.draw(globalCloth);
		window.display();
	}

	sf::Vector2i mpos;
	unsigned int choose;
	while (window.isOpen()) {
		mpos = sf::Mouse::getPosition(window);
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					goto leaveList;
					break;
				default:
					break;
				}
				break;
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			case sf::Event::MouseEntered:
			case sf::Event::MouseLeft:
			case sf::Event::MouseMoved:
				choose = graphicBtn_void_btn_0->everyCheck(event, mpos);
				if (choose & 0x8000) {
					switch (choose & 0x7FFF) {
					case 0:
						audioVoid.addSound(0);
						goto leaveList;
						break;
					default:;
					}
				}
				break;
			default:;
			}
		}
		window.clear(sf::Color::White);
		graphicRoot_void_0.displayIn(window);
		window.display();
	}
leaveList:
	if (window.isOpen()) {
		clock.restart();
		while ((timer = clock.getElapsedTime().asSeconds()) < 0.5f) {
			while (window.pollEvent(event))
				;
			audioVoid.getBGM()->setVolume(universialVolumn * std::max((0.25f - timer) / 0.25f, 0.0f));
			rhodesCloth.setColor(sf::Color(255, 255, 255, 255 - (int)(255 * std::max((0.15f - timer) / 0.15f, 0.0f))));
			window.clear(sf::Color::White);
			graphicRoot_void_0.displayIn(window);
			window.draw(rhodesCloth);
			window.display();
		}
	}
	audioVoid.reset();
	return;
}

bool title(sf::RenderWindow &window) {
	sf::Clock clock;
	float timer;
	sf::Event event;

	OHMS::GSROOT graphicRoot_title_0;

	sf::Texture texture_ui_title_0;
	texture_ui_title_0.loadFromFile("assets/ui/texture/titleback.png");
	sf::Vector2u texture_ui_title_0_Size = texture_ui_title_0.getSize();

	sf::Texture texture_ui_title_des_0;
	texture_ui_title_des_0.loadFromFile("assets/ui/texture/describe/des0.png");
	sf::Vector2u texture_ui_title_des_0_Size = texture_ui_title_des_0.getSize();

	sf::Texture texture_ui_title_des_1;
	texture_ui_title_des_1.loadFromFile("assets/ui/texture/describe/des1.png");
	sf::Vector2u texture_ui_title_des_1_Size = texture_ui_title_des_1.getSize();

	OHMS::GSsprite* graphicSprite_title_0 = new OHMS::GSsprite(texture_ui_title_0);
	OHMS::GSsprite* graphicSprite_title_des_0 = new OHMS::GSsprite(texture_ui_title_des_0);
	OHMS::GStext * graphicText_title_version_0 = new OHMS::GStext;
	graphicRoot_title_0.setSubGS(0, graphicSprite_title_0);
	graphicRoot_title_0.setSubGS(1, graphicText_title_version_0);
	graphicSprite_title_0->setSubGS(1, graphicSprite_title_des_0);

	graphicSprite_title_0->setOrgRate({ 0.5f, 0.5f });
	graphicSprite_title_0->getSprite()->scale(winWidth * 1.0f / texture_ui_title_0_Size.x, winHight * 0.8f / texture_ui_title_0_Size.y);

	graphicSprite_title_des_0->setOrgRate({ 0.5f, 0.5f });

	graphicText_title_version_0->setPosToParent({ winWidth * -0.49f, winHight * -0.45f });
	sf::Text * text_title_Version = graphicText_title_version_0->getText();
	text_title_Version->setCharacterSize(30);
	text_title_Version->setFillColor(sf::Color::White);
	text_title_Version->setFont(ui_font);
	text_title_Version->setString((sf::String)"Ver: " + ui_version);
	text_title_Version->setStyle(sf::Text::Style::Italic);

	graphicRoot_title_0.setPosAbs({ winWidth * 0.5f, winHight * 0.5f });

	OHMS::AudioCtrl audioMain_title_0(1, 2); // Load Title Audio 
	audioMain_title_0.setBGM(0, "assets/audio/music/", "title");
	audioMain_title_0.setSound(0, "assets/audio/sounds/", "ui_btn");
	audioMain_title_0.setSound(1, "assets/character/" + OHMS::getRandOperator(characterCnt, 4) + "/sounds/", "arknights");

	audioMain_title_0.setVolumn(universialVolumn);

	audioMain_title_0.playBGM(0);

	clock.restart();
	while ((timer = clock.getElapsedTime().asSeconds()) < 0.2) {
		while (window.pollEvent(event))
			;
		window.clear();
		graphicRoot_title_0.displayIn(window);
		globalCloth.setColor(sf::Color(0, 0, 0, (int)(255 * std::max((0.2f - timer) / 0.2f, 0.0f))));
		window.draw(globalCloth);
		window.display();
	}

	bool describe = false;
	timer = clock.restart().asSeconds();
	while (window.isOpen()) { // Title Loop 
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed)
				audioMain_title_0.addSound(0);
			if (event.type == sf::Event::MouseButtonReleased)
				goto finiTitle;
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					window.close();
					break;
				default:
					break;
				}
			}
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		if ((timer = clock.getElapsedTime().asSeconds()) > 3.63f) {
			clock.restart();
			if (describe) {
				graphicSprite_title_des_0->getSprite()->setTexture(texture_ui_title_des_0);
			}
			else {
				graphicSprite_title_des_0->getSprite()->setTexture(texture_ui_title_des_1);
			}
			describe = !describe;
		}
		window.clear();
		graphicRoot_title_0.displayIn(window);
		window.display();
	}
finiTitle:
	if (window.isOpen()) {
		clock.restart();
		while ((timer = clock.getElapsedTime().asSeconds()) < 0.8f) {
			while (window.pollEvent(event))
				;
			audioMain_title_0.getBGM()->setVolume(universialVolumn * std::max((0.15f - timer) / 0.15f, 0.0f));
			globalCloth.setColor(sf::Color(0, 0, 0, 255 - (int)(255 * std::max((0.15f - timer) / 0.15f, 0.0f))));
			window.clear();
			graphicRoot_title_0.displayIn(window);
			window.draw(globalCloth);
			window.display();
		}
	}
	audioMain_title_0.reset();

	OHMS::GSROOT graphicRoot_title_1;

	sf::Texture texture_ui_title_1;
	texture_ui_title_1.loadFromFile("assets/ui/texture/trans_bg0.png");
	sf::Vector2u texture_ui_title_1_size = texture_ui_title_1.getSize();

	sf::Texture texture_ui_title_2;
	texture_ui_title_2.loadFromFile("assets/ui/texture/trans_bg1.png");
	sf::Vector2u texture_ui_title_2_size = texture_ui_title_2.getSize();

	OHMS::GSsprite *graphicSprite_log_0 = new OHMS::GSsprite(texture_ui_title_1);
	OHMS::GSsprite *graphicSprite_log_1 = new OHMS::GSsprite(texture_ui_title_2);
	OHMS::GStext *graphicText_title_log_0 = new OHMS::GStext;
	OHMS::GStext *graphicText_title_log_1 = new OHMS::GStext;
	OHMS::GSbuttonArray * startButton = new OHMS::GSbuttonArray;
	graphicRoot_title_1.setSubGS(0, graphicSprite_log_1);
	graphicRoot_title_1.setSubGS(1, graphicSprite_log_0);
	graphicSprite_log_0->setSubGS(1, graphicText_title_log_0);
	graphicText_title_log_0->setSubGS(0, graphicText_title_log_1);
	graphicText_title_log_0->setSubGS(1, startButton);

	graphicSprite_log_0->setOrgRate({ 0.5f, 0.5f });
	graphicSprite_log_0->getSprite()->scale(winWidth * 1.0f / texture_ui_title_1_size.x, winHight * 0.8f / texture_ui_title_1_size.y);

	graphicSprite_log_1->setOrgRate({ 0.5f, 0.5f });
	graphicSprite_log_1->getSprite()->scale(winWidth * 1.0f / texture_ui_title_2_size.x, winHight * 0.8f / texture_ui_title_2_size.y);

	graphicText_title_log_0->setOrgRate({ 0.5f, 1.0f });
	graphicText_title_log_0->setPosToParent({ 0.0f, -50.0f });
	sf::Text *text_title_name_0 = graphicText_title_log_0->getText();
	text_title_name_0->setCharacterSize(150);
	text_title_name_0->setFillColor(sf::Color::Black);
	text_title_name_0->setFont(sp_font);
	text_title_name_0->setString(L"昨日方舟");
	graphicText_title_log_0->updateOrg();

	graphicText_title_log_1->setOrgRate({ 0.5f, 0.0f });
	graphicText_title_log_1->setPosToParent({ 0.0f, 20.0f });
	sf::Text *text_title_name_1 = graphicText_title_log_1->getText();
	text_title_name_1->setCharacterSize(60);
	text_title_name_1->setFillColor(sf::Color::Black);
	text_title_name_1->setFont(ui_font);
	text_title_name_1->setString(L"ARCHKNIGHTS");
	graphicText_title_log_1->updateOrg();

	sf::Image img;
	img.create(150, 70, sf::Color::Black);
	sf::Texture btn;
	btn.loadFromImage(img);

	startButton->initialize(&window);
	startButton->push_back(&window, btn);
	startButton->setPosition(0, winWidth / 2.0f - 80.0f, winHight / 2.0f + 195.0f);
	startButton->setStyle(0, 200, 140);
	startButton->setText(0, ui_font, L"开始唤醒", 25);
	startButton->setTextPosition(0, 26, 15);
	startButton->isActive = true;

	sf::RectangleShape line1(sf::Vector2f(800.0f, 8.0f));
	line1.setOrigin(800.0f, 4.0f);
	line1.setFillColor(sf::Color(255, 220, 0));
	line1.setPosition(-5.0f, 600.0f);
	sf::RectangleShape line2(sf::Vector2f(800.0f, 8.0f));
	line2.setOrigin(0.0f, 4.0f);
	line2.setFillColor(sf::Color(255, 220, 0));
	line2.setPosition(winWidth + 5.0f, 600.0f);

	graphicRoot_title_1.setPosAbs({ winWidth * 0.5f, winHight * 0.5f });

	clock.restart();
	while ((timer = clock.getElapsedTime().asSeconds()) < 0.3) {
		while (window.pollEvent(event));
		window.clear(sf::Color(120, 120, 120));
		graphicRoot_title_1.displayIn(window);
		globalCloth.setColor(sf::Color(0, 0, 0, (int)(255 * std::max((0.2f - timer) / 0.2f, 0.0f))));
		window.draw(globalCloth);
		window.display();
	}

	audioMain_title_0.getBGM()->setVolume(universialVolumn);
	audioMain_title_0.changeBGM(0);

	sf::Vector2i mpos;
	unsigned int choose;
	while (window.isOpen()) { // Login Loop 
		mpos = sf::Mouse::getPosition(window);
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			case sf::Event::MouseEntered:
			case sf::Event::MouseLeft:
			case sf::Event::MouseMoved:
				choose = startButton->everyCheck(event, mpos);
				if (choose & 0x8000) {
					switch (choose & 0x7FFF) {
					case 0:
						audioMain_title_0.addSound(0);
						clock.restart();
						while ((timer = clock.getElapsedTime().asSeconds()) < 0.01f);
						audioMain_title_0.addSound(1);
						goto finiLog;
						break;
					default:;
					}
				}
				break;
			}
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Escape: 
					window.close();
					break;
				default:
					break;
				}
			}
		}
		window.clear(sf::Color(80,80,80));
		graphicRoot_title_1.displayIn(window);
		window.display();
	}
finiLog:
	if (window.isOpen()) {
		text_title_name_0->setFillColor(sf::Color(0, 0, 0, 0));
		text_title_name_1->setFillColor(sf::Color(0, 0, 0, 0));
		startButton->isActive = false;
		clock.restart();
		sf::Text text1, text2;
		text1.setFillColor(sf::Color(255, 220, 0));
		text2.setFillColor(sf::Color(255, 220, 0));
		text1.setString("0%");
		text2.setString("0%");
		text1.setFont(ui_font);
		text2.setFont(ui_font);
		text1.setCharacterSize(25);
		text2.setCharacterSize(25);
		while ((timer = clock.getElapsedTime().asSeconds()) < 3.0f ) {/* || (audioMain_title_0.getSound()->getStatus() == sf::Sound::Status::Playing)*/
			while (window.pollEvent(event))
				;
			if( timer < 1.0f ){
				graphicSprite_log_0->getSprite()->
					setColor(sf::Color(255, 255, 255, (int)(255 * std::max((0.15f - timer) / 0.15f, 0.0f))));
			}
			float loadRate = std::min(timer, 0.1f);
			loadRate = std::max(loadRate, std::min(timer - 0.2f, 0.5f) + 0.1f);
			loadRate = std::max(loadRate, std::min(timer - 2.4f , 0.3f) + 0.6f);
			loadRate = std::max(loadRate, std::min(timer - 2.8f , 0.1f) + 0.9f);
			line1.setPosition(winWidth * 0.5f * loadRate, 600.0f);
			text1.setPosition(winWidth * 0.5f * loadRate - 20, 565.0f);
			line2.setPosition(winWidth * (1.0f - 0.5f * loadRate), 600.0f);
			text2.setPosition(winWidth * (1.0f - 0.5f * loadRate) - 20, 565.0f);
			text1.setString(std::to_string((int)(loadRate * 100.0f)) + "%");
			text2.setString(std::to_string((int)(loadRate * 100.0f)) + "%");
			window.clear();
			graphicRoot_title_1.displayIn(window);
			window.draw(line1);
			window.draw(line2);
			window.draw(text1);
			window.draw(text2);
			window.display();
		}
		clock.restart();
		while ((timer = clock.getElapsedTime().asSeconds()) < 2.0f) {
			while (window.pollEvent(event))
				;
			if (timer > 1.6f) {
				audioMain_title_0.getBGM()->setVolume(universialVolumn * std::max((1.75f - timer) / 0.15f, 0.0f));
			}
			rhodesCloth.setColor(sf::Color(255, 255, 255, (int)(std::min(timer, 0.1f) / 0.1f * 255.1f)));
			window.clear();
			graphicRoot_title_1.displayIn(window);
			window.draw(rhodesCloth);
			window.display();
		}
	}
	audioMain_title_0.reset();
	return false;
}

void mainMenu(sf::RenderWindow &window) {
	srand((unsigned int)time(0));
	sf::Clock clock;
	float timer;
	sf::Event event;

	unsigned int characterChoice = 6;

	OHMS::AudioCtrl audioVoid(1, 5); // Load Main Menu Audio 
	audioVoid.setBGM(0, "assets/audio/music/", "void");
	audioVoid.setSound(0, "assets/audio/sounds/", "ui_btn");
	audioVoid.setSound(1, "assets/character/" + OHMS::getOperatorStr(characterChoice) + "/sounds/", "hello");
	audioVoid.setSound(2, "assets/audio/sounds/", "ui_back");
	audioVoid.setSound(3, "assets/audio/sounds/", "ui_confirm");
	audioVoid.setSound(4, "assets/audio/sounds/", "ui_gamestart");
	audioVoid.setVolumn(universialVolumn);

	sf::Image img;
	img.create(300, 120, sf::Color(255, 255, 255));

	sf::Texture voidBack, assistant, btn;
	voidBack.loadFromFile("assets/ui/texture/bkp.png");
	assistant.loadFromFile("assets/character/" + OHMS::getOperatorStr(characterChoice) + "/picture/main.png");
	assistant.setSmooth(true);
	btn.loadFromImage(img);
	unsigned int middleX, faceY, middleY;
	OHMS::filescanf("assets/character/" + OHMS::getOperatorStr(characterChoice) + "/picture/info.txt", "%d%d%d", &middleX, &middleY, &faceY);

	float rate = winHight * 1.0f / voidBack.getSize().y;

	OHMS::GSROOT graphicRoot_void_0;

	OHMS::GSsprite *graphicSprite_void_back_0 = new OHMS::GSsprite(voidBack);
	OHMS::GSsprite *graphicSprite_void_assist_0 = new OHMS::GSsprite(assistant);
	OHMS::GSbuttonArray * graphicBtn_void_btn_0 = new OHMS::GSbuttonArray;
	graphicSprite_void_back_0->setOrgRate({ 0.5f, 0.5f });
	graphicSprite_void_back_0->getSprite()->setScale(rate, rate);

	graphicRoot_void_0.setSubGS(0, graphicSprite_void_back_0);
	graphicRoot_void_0.setSubGS(1, graphicSprite_void_assist_0);
	graphicSprite_void_assist_0->setSubGS(0, graphicBtn_void_btn_0);

	graphicRoot_void_0.setPosAbs({ winWidth / 2.0f, winHight / 2.0f });

	graphicSprite_void_assist_0->getSprite()->setOrigin((float)middleX, (float)middleY);
	float assistRate = 200.0f / (middleY - faceY);
	graphicSprite_void_assist_0->getSprite()->setScale(assistRate, assistRate);

	graphicBtn_void_btn_0->initialize(&window);
	graphicBtn_void_btn_0->push_back(&window, btn);
	graphicBtn_void_btn_0->setPosition(0, 810.0f, 200.0f);
	graphicBtn_void_btn_0->setStyle(0, 200, 140);
	graphicBtn_void_btn_0->setText(0, ui_font, L"作战", 80);
	graphicBtn_void_btn_0->setTextPosition(0, 65, 10);

	graphicBtn_void_btn_0->push_back(&window, btn);
	graphicBtn_void_btn_0->setPosition(1, 5.0f, 5.0f);
	graphicBtn_void_btn_0->setStyle(1, 200, 140);
	graphicBtn_void_btn_0->setText(1, ui_font, L"info", 60);
	graphicBtn_void_btn_0->setTextPosition(1, 32, 10);
	graphicBtn_void_btn_0->setScale(1, 0.3f, 0.3f);

	graphicSprite_void_assist_0->setPosToParent({ -340.0f, -40.0f });
	graphicBtn_void_btn_0->isActive = true;

	clock.restart();
	while ((timer = clock.getElapsedTime().asSeconds()) < 0.2f) {
		while (window.pollEvent(event))
			;
		rhodesCloth.setColor(sf::Color(255, 255, 255, 255 - (int)(std::min(timer, 0.1f) / 0.1f * 255.1f)));
		window.clear();
		graphicRoot_void_0.displayIn(window);
		window.draw(rhodesCloth);
		window.display();
	}

	audioVoid.playBGM(0);
	audioVoid.addSound(1);

	sf::Vector2i mpos;
	unsigned int choose;
	while (window.isOpen()) {
		mpos = sf::Mouse::getPosition(window);
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					window.close();
					break;
				default:;
				}
				break;
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			case sf::Event::MouseEntered:
			case sf::Event::MouseLeft:
			case sf::Event::MouseMoved:
				choose = graphicBtn_void_btn_0->everyCheck(event, mpos);
				if (choose & 0x8000) {
					switch (choose & 0x7FFF) {
					case 0:
						audioVoid.addSound(0);
						mapMenu(window, audioVoid);
						if (GOtoBattle)
							goto leave;
						break;
					case 1:
						audioVoid.addSound(0);
						GOtoList = true;
						clock.restart();
						while ((timer = clock.getElapsedTime().asSeconds()) < 0.4f) {
							while (window.pollEvent(event));
							audioVoid.getBGM()->setVolume(universialVolumn * std::max((0.25f - timer) / 0.25f, 0.0f));
							globalCloth.setColor(sf::Color(0, 0, 0, 255 - (int)(255 * std::max((0.15f - timer) / 0.15f, 0.0f))));
							window.clear();
							graphicRoot_void_0.displayIn(window);
							window.draw(globalCloth);
							window.display();
						}
						goto leave;
						break;
					default:;
					}
				}
				break;
			default:;
			}
		}
		window.clear();
		graphicRoot_void_0.displayIn(window);
		window.display();
	}
leave:
	audioVoid.reset();
	return;
}

void mapMenu(sf::RenderWindow &window, OHMS::AudioCtrl &ac) {
	sf::Texture lastCloth_TX;
	lastCloth_TX.create(winWidth, winHight);
	lastCloth_TX.update(window);
	sf::Sprite lastCloth;
	lastCloth.setTexture(lastCloth_TX);

	sf::Clock clock;
	float timer;
	sf::Event event;

	sf::Image img;
	img.create(100, 60, sf::Color(0, 0, 0));

	sf::Texture voidBack, assistant, btn;
	voidBack.loadFromFile("assets/ui/texture/bkp.png");
	btn.loadFromImage(img);

	OHMS::GSROOT graphicRoot_sakusen_0;

	float rate = winHight * 1.0f / voidBack.getSize().y;

	OHMS::GSsprite *graphicSprite_void_back_0 = new OHMS::GSsprite(voidBack);
	OHMS::GSbuttonArray * graphicBtn_void_btn_0 = new OHMS::GSbuttonArray;
	graphicSprite_void_back_0->setOrgRate({ 0.5f, 0.5f });
	graphicSprite_void_back_0->getSprite()->setScale(rate, rate);
	graphicSprite_void_back_0->getSprite()->setColor(sf::Color(255, 255, 255, 64));

	graphicRoot_sakusen_0.setSubGS(0, graphicSprite_void_back_0);
	graphicRoot_sakusen_0.setSubGS(1, graphicBtn_void_btn_0);

	graphicRoot_sakusen_0.setPosAbs({ winWidth / 2.0f, winHight / 2.0f });

	graphicBtn_void_btn_0->initialize(&window);

	graphicBtn_void_btn_0->push_back(&window, btn);
	graphicBtn_void_btn_0->setPosition(0, 5.0f, 5.0f);
	graphicBtn_void_btn_0->setStyle(0, 200, 140);
	graphicBtn_void_btn_0->setText(0, ui_font, L"<", 30);
	graphicBtn_void_btn_0->setTextPosition(0, 42, 10);

	graphicBtn_void_btn_0->push_back(&window, btn);
	graphicBtn_void_btn_0->setPosition(1, 560.0f, 300.0f);
	graphicBtn_void_btn_0->setStyle(1, 200, 140);
	graphicBtn_void_btn_0->setText(1, ui_font, L"7-4", 30);
	graphicBtn_void_btn_0->setTextPosition(1, 30, 10);

	graphicBtn_void_btn_0->push_back(&window, btn);
	graphicBtn_void_btn_0->setPosition(2, 560.0f, 400.0f);
	graphicBtn_void_btn_0->setStyle(2, 200, 140);
	graphicBtn_void_btn_0->setText(2, ui_font, L"EX", 30);
	graphicBtn_void_btn_0->setTextPosition(2, 30, 10);

	graphicBtn_void_btn_0->push_back(&window, btn);
	graphicBtn_void_btn_0->setPosition(3, 560.0f, 200.0f);
	graphicBtn_void_btn_0->setStyle(3, 200, 140);
	graphicBtn_void_btn_0->setText(3, ui_font, L"0-1", 30);
	graphicBtn_void_btn_0->setTextPosition(3, 30, 10);

	graphicBtn_void_btn_0->isActive = true;

	clock.restart();
	while ((timer = clock.getElapsedTime().asSeconds()) < 0.2f) {
		while (window.pollEvent(event))
			;
		lastCloth.setColor(sf::Color(255, 255, 255, 255 - (int)(std::min(timer, 0.15f) / 0.15f * 255.1f)));
		window.clear(sf::Color::White);
		graphicRoot_sakusen_0.displayIn(window);
		window.draw(lastCloth);
		window.display();
	}

	sf::Vector2i mpos;
	unsigned int choose;
	while (window.isOpen()) {
		mpos = sf::Mouse::getPosition(window);
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					GOtoBattle = false;
					clock.restart();
					while ((timer = clock.getElapsedTime().asSeconds()) < 0.2f) {
						while (window.pollEvent(event));
						lastCloth.setColor(sf::Color(255, 255, 255, (int)(std::min(timer, 0.15f) / 0.15f * 255.1f)));
						window.clear(sf::Color::White);
						graphicRoot_sakusen_0.displayIn(window);
						window.draw(lastCloth);
						window.display();
					}
					return;
					break;
				default:
					break;
				}
				break;
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			case sf::Event::MouseEntered:
			case sf::Event::MouseLeft:
			case sf::Event::MouseMoved:
				choose = graphicBtn_void_btn_0->everyCheck(event, mpos);
				if (choose & 0x8000) {
					switch (choose & 0x7FFF) {
					case 0:
						ac.addSound(2);
						GOtoBattle = false;
						clock.restart();
						while ((timer = clock.getElapsedTime().asSeconds()) < 0.2f) {
							while (window.pollEvent(event));
							lastCloth.setColor(sf::Color(255, 255, 255, (int)(std::min(timer, 0.15f) / 0.15f * 255.1f)));
							window.clear(sf::Color::White);
							graphicRoot_sakusen_0.displayIn(window);
							window.draw(lastCloth);
							window.display();
						}
						return;
						break;
					case 1:
						ac.addSound(4);
						tipPop(window);
						if (BattleCtrl(window, ac)) {
							GOtoBattle = true;
							levelName = OHMS::getOperatorStr(1);
							goto goBattle;
						}
						break;
					case 2:
						ac.addSound(4);
						if (BattleCtrl(window, ac)) {
							GOtoBattle = true;
							levelName = "ex" + OHMS::getOperatorStr(0);
							goto goBattle;
						}
						break;
					case 3:
						ac.addSound(4);
						tipPop(window);
						if (BattleCtrl(window, ac)) {
							GOtoBattle = true;
							levelName = OHMS::getOperatorStr(0);
							goto goBattle;
						}
						break;
					default:;
					}
				}
				break;
			default:;
			}
		}
		window.clear(sf::Color::White);
		graphicRoot_sakusen_0.displayIn(window);
		window.display();
	}
goBattle:
	return;
}

bool BattleCtrl(sf::RenderWindow& window, OHMS::AudioCtrl& ac) {
	sf::Texture lastCloth_TX;
	lastCloth_TX.create(winWidth, winHight);
	lastCloth_TX.update(window);
	sf::Sprite lastCloth;
	lastCloth.setTexture(lastCloth_TX);

	sf::Clock clock;
	float timer;
	sf::Event event;
	sf::Vector2i mpos;
	unsigned int choose;

	sf::Texture testTx;
	testTx.loadFromFile("assets/ui/texture/inbattle.png");
	testTx.setSmooth(true);
	sf::Vector2u testTxSize = testTx.getSize();

	sf::Image img;
	img.create(100, 60, sf::Color(0, 0, 0));
	sf::Texture btn, startBattle;
	btn.loadFromImage(img);
	startBattle.loadFromFile("assets/ui/texture/startbattle.png");
	startBattle.setSmooth(true);

	OHMS::GSROOT graphicRoot_sakusen_0;

	OHMS::GSsprite* graphicSprite_void_back_0 = new OHMS::GSsprite(testTx);
	OHMS::GSbuttonArray* graphicBtn_void_btn_0 = new OHMS::GSbuttonArray;
	graphicSprite_void_back_0->setOrgRate({ 0.5f, 0.5f });
	graphicSprite_void_back_0->getSprite()->setScale(0.8f, 0.8f);

	graphicRoot_sakusen_0.setSubGS(0, graphicSprite_void_back_0);
	graphicRoot_sakusen_0.setSubGS(1, graphicBtn_void_btn_0);

	graphicRoot_sakusen_0.setPosAbs({ winWidth / 2.0f, winHight / 2.0f });

	graphicBtn_void_btn_0->initialize(&window);

	graphicBtn_void_btn_0->push_back(&window, btn);
	graphicBtn_void_btn_0->setPosition(0, 5.0f, 5.0f);
	graphicBtn_void_btn_0->setStyle(0, 200, 140);
	graphicBtn_void_btn_0->setText(0, ui_font, L"<", 30);
	graphicBtn_void_btn_0->setTextPosition(0, 42, 10);

	graphicBtn_void_btn_0->push_back(&window, startBattle);
	graphicBtn_void_btn_0->setPosition(1, 1064.0f, 450.0f);
	graphicBtn_void_btn_0->setScale(1, 0.8f, 0.8f);
	graphicBtn_void_btn_0->setStyle(1, 200, 140);

	graphicBtn_void_btn_0->isActive = true;

	clock.restart();
	while ((timer = clock.getElapsedTime().asSeconds()) < 0.2f) {
		while (window.pollEvent(event))
			;
		lastCloth.setColor(sf::Color(255, 255, 255, 255 - (int)(std::min(timer, 0.15f) / 0.15f * 255.1f)));
		window.clear(sf::Color::White);
		graphicRoot_sakusen_0.displayIn(window);
		window.draw(lastCloth);
		window.display();
	}

	while (window.isOpen()) {
		mpos = sf::Mouse::getPosition(window);
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					clock.restart();
					while ((timer = clock.getElapsedTime().asSeconds()) < 0.2f) {
						while (window.pollEvent(event));
						lastCloth.setColor(sf::Color(255, 255, 255, (int)(std::min(timer, 0.15f) / 0.15f * 255.1f)));
						window.clear(sf::Color::White);
						graphicRoot_sakusen_0.displayIn(window);
						window.draw(lastCloth);
						window.display();
					}
					return false;
					break;
				default:
					break;
				}
				break;
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			case sf::Event::MouseEntered:
			case sf::Event::MouseLeft:
			case sf::Event::MouseMoved:
				choose = graphicBtn_void_btn_0->everyCheck(event, mpos);
				if (choose & 0x8000) {
					switch (choose & 0x7FFF) {
					case 0:
						ac.addSound(2);
						clock.restart();
						while ((timer = clock.getElapsedTime().asSeconds()) < 0.2f) {
							while (window.pollEvent(event));
							lastCloth.setColor(sf::Color(255, 255, 255, (int)(std::min(timer, 0.15f) / 0.15f * 255.1f)));
							window.clear(sf::Color::White);
							graphicRoot_sakusen_0.displayIn(window);
							window.draw(lastCloth);
							window.display();
						}
						return false;
						break;
					case 1:
						ac.addSound(3);
						goto battleCtrl_over;
						break;
					default:;
					}
				}
				break;
			default:;
			}
		}
		window.clear(sf::Color::White);
		graphicRoot_sakusen_0.displayIn(window);
		window.display();
	}

battleCtrl_over:
	if (window.isOpen()) {
		clock.restart();
		while ((timer = clock.getElapsedTime().asSeconds()) < 0.8f) {
			while (window.pollEvent(event))
				;
			ac.getBGM()->setVolume(universialVolumn * std::max((0.25f - timer) / 0.25f, 0.0f));
			globalCloth.setColor(sf::Color(0, 0, 0, 255 - (int)(255 * std::max((0.25f - timer) / 0.25f, 0.0f))));
			window.clear(sf::Color::White);
			graphicRoot_sakusen_0.displayIn(window);
			window.draw(globalCloth);
			window.display();
		}
	}
	return true;
}

void BattleMain(sf::RenderWindow &window, std::string LVname) {
	int rank = 3;

	sf::Clock clock;
	float timer;
	clock.restart();
	unsigned int frameBuffer = 0;
	sf::Event event;
	OHMS::GSROOT bgUI;
	OHMS::BattleGroundMain bg;

	bg.readFromFile("assets/level/" + LVname + "/");
	bg.setFont(&ui_font);

	OHMS::GStext * GS_levelID, *GS_levelName, *GS_operation;
	GS_levelID = new OHMS::GStext;
	GS_levelName = new OHMS::GStext;
	GS_operation = new OHMS::GStext;
	bgUI.setSubGS(1, GS_levelID);
	GS_levelID->setSubGS(1, GS_levelName);
	GS_levelID->setSubGS(0, GS_operation);

	sf::Text * tempText;

	tempText = GS_levelID->getText();
	tempText->setFont(li_font);
	tempText->setCharacterSize(100);
	tempText->setString(bg.levelID);
	GS_levelID->setOrgRate({ 0.5f, 0.6f });

	tempText = GS_levelName->getText();
	tempText->setFont(ui_font);
	tempText->setCharacterSize(60);
	setStr(tempText, bg.levelName.c_str());
	GS_levelName->setOrgRate({ 0.5f, -0.2f });
	GS_levelName->setPosToParent({ 0.0f, 50.0f });

	tempText = GS_operation->getText();
	tempText->setFont(li_font);
	tempText->setCharacterSize(30);
	tempText->setString("OPERATION");
	GS_operation->setOrgRate({ 0.5f, 1.0f });
	GS_operation->setPosToParent({ 0.0f, -40.0f });

	bgUI.setPosAbs({ winWidth / 2.0f, winHight / 2.0f });

	clock.restart();
	while ( (timer = clock.getElapsedTime().asSeconds()) < 0.21f ) {
		while (window.pollEvent(event))
			;
		globalCloth.setColor(sf::Color(0, 0, 0, 255 - (int)(std::min(timer, 0.2f) / 0.2f * 255.1f)));
		window.clear();
		bgUI.displayIn(window);
		window.draw(globalCloth);
		window.display();
	}

	sf::Vector2u size = bg.getBattleGroundSize();

	bg.initEntityOrg(11, 4);

	bg.loadCanPlayList();

	bg.myAudio->setBGMVolumn(universialVolumn);
	bg.myAudio->setSoundVolumn(universialVolumn * 1.8f);

	sf::Vector2f center = { winWidth / 2.0f, winHight / 2.0f };

	bg.setEntityOrg(0, 0, "assets/character/" + OHMS::getOperatorStr(0) + "/");
	bg.setEntityOrg(0, 1, "assets/character/" + OHMS::getOperatorStr(1) + "/");
	bg.setEntityOrg(0, 2, "assets/character/" + OHMS::getOperatorStr(2) + "/");
	bg.setEntityOrg(0, 3, "assets/character/" + OHMS::getOperatorStr(3) + "/");
	bg.setEntityOrg(0, 4, "assets/character/" + OHMS::getOperatorStr(4) + "/");
	bg.setEntityOrg(0, 5, "assets/character/" + OHMS::getOperatorStr(5) + "/");
	bg.setEntityOrg(0, 6, "assets/character/" + OHMS::getOperatorStr(6) + "/");
	bg.setEntityOrg(0, 7, "assets/character/" + OHMS::getOperatorStr(7) + "/");
	bg.setEntityOrg(0, 8, "assets/character/" + OHMS::getOperatorStr(8) + "/");
	bg.setEntityOrg(0, 9, "assets/character/" + OHMS::getOperatorStr(9) + "/");
	bg.setEntityOrg(0, 10, "assets/character/" + OHMS::getOperatorStr(10) + "/");

	clock.restart();
	while ((timer = clock.getElapsedTime().asSeconds()) < 0.21f) {
		while (window.pollEvent(event))
			;
		globalCloth.setColor(sf::Color(0, 0, 0, 255 - (int)(std::min(timer, 0.2f) / 0.2f * 255.1f)));
		window.clear();
		bg.displayIn(window);
		window.draw(globalCloth);
		bgUI.displayIn(window);
		window.display();
	}

	bg.setEntityOrg(1, 0, "assets/battle/enemy/" + OHMS::getOperatorStr(0) + "/");
	bg.setEntityOrg(1, 1, "assets/battle/enemy/" + OHMS::getOperatorStr(1) + "/");
	bg.setEntityOrg(1, 2, "assets/battle/enemy/" + OHMS::getOperatorStr(2) + "/");
	bg.setEntityOrg(1, 3, "assets/battle/enemy/" + OHMS::getOperatorStr(3) + "/");

	clock.restart();
	while ((timer = clock.getElapsedTime().asSeconds()) < 0.31f) {
		while (window.pollEvent(event))
			;
		bgUI.setAlphaAbs(255 - (int)(std::min(timer, 0.3f) / 0.3f * 254.9f));
		window.clear();
		bg.displayIn(window);
		bgUI.displayIn(window);
		window.display();
	}

	bg.myAudio->playBGM(0);

	sf::Vector2f testPos = { 0.5f, 0.5f };

	OHMS::EntityMain* testE = nullptr;

	bg.op->cclFocus();
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			bg.op->opc->check(event, sf::Mouse::getPosition(window));
			bg.op->check(event, sf::Mouse::getPosition(window));
			switch (event.type) {
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					bg.rank = 1;
					goto GameOver;
					break;
				}
				break;
			default:;
			}
		}

		frameBuffer++;
		if (frameBuffer < bg.frameBufferMax)
			goto compend;
		bg.update();
		if (bg.over) break;
		frameBuffer = 0;

	compend:
		window.clear();
		bg.displayIn(window);
		window.display();
	}

GameOver:
	rank = bg.rank;

	sf::Texture battleOverTX;
	battleOverTX.create(winWidth, winHight);
	battleOverTX.update(window);
	sf::Sprite battleOverSp;
	battleOverSp.setTexture(battleOverTX);

	sf::Texture overTX;
	overTX.loadFromFile("assets/battle/texture/finish.png");
	overTX.setSmooth(true);
	sf::Sprite overSP, overSP1;
	overSP.setTexture(overTX);
	overSP1.setTexture(overTX);

	if (rank >= 2) {
		sf::Image imtmp;
		imtmp.create(3000, 500, sf::Color(25, 25, 25, 180));
		sf::Texture txtmp;
		txtmp.loadFromImage(imtmp);
		overSP1.setTexture(txtmp, true);

		overSP.setTextureRect(sf::IntRect(0, 1522, 1518, 526));
		overSP.setOrigin(759.0f, 263.0f);
		overSP.setScale(0.6f, 0.6f);
		overSP.setPosition(-500.0f, winHight / 2.0f);

		overSP1.setOrigin(1500.0f, 250.0f);
		overSP1.setScale(0.6f, 0.6f);
		overSP1.setPosition(winWidth / 2.0f, winHight / 2.0f);

		bg.myAudio->setBGMVolumn(0.0f);
		bg.myAudio->addSound(1);
		clock.restart();
		while ((timer = clock.getElapsedTime().asSeconds()) < 1.5f) {
			while (window.pollEvent(event));
			if (timer < 0.3f) {
				overSP.setPosition(winWidth / 2.0f * (std::min(timer, 0.2f) / 0.1f - 1.0f), winHight / 2.0f);
				overSP1.setColor(sf::Color(255, 255, 255, (int)(255.9f * (std::min(timer, 0.2f) / 0.2f))));
			}
			else if (timer > 0.8f) {
				overSP.setPosition(winWidth / 2.0f * ((timer - 0.8f) / 0.1f + 1.0f), winHight / 2.0f);
				overSP1.setColor(sf::Color(255, 255, 255, 255 - (int)(255.9f * (std::min(timer - 0.8f, 0.2f) / 0.2f))));
			}
			window.clear();
			window.draw(battleOverSp);
			window.draw(overSP1);
			window.draw(overSP);
			window.display();
		}
		goto endupWithoutAudioFadingAway;
	}
	else if (rank < 2) {
		overSP.setTextureRect(sf::IntRect(0, 628, 567, 582));
		overSP.setOrigin(283.5f, 291.0f);
		overSP.setPosition(winWidth / 2.0f, winHight / 2.0f);

		overSP1.setTexture(overTX);
		overSP1.setTextureRect(sf::IntRect(567, 696, 514, 514));
		overSP1.setScale(winWidth / 514.0f, winHight / 514.0f);

		if (rank == 0) {
			bg.myAudio->changeBGM(2);
		}
		else {
			bg.myAudio->changeBGM(3);
		}
		clock.restart();
		while ((timer = clock.getElapsedTime().asSeconds()) < 0.2f) {
			while (window.pollEvent(event))
				;
			overSP1.setColor(sf::Color(255, 255, 255, (int)(255.9f * std::min(timer / 0.1f, 1.0f))));
			overSP.setColor(sf::Color(255, 255, 255, (int)(255.9f * std::min(timer / 0.1f, 1.0f))));
			window.clear();
			window.draw(battleOverSp);
			window.draw(overSP1);
			window.draw(overSP);
			window.display();
		}
		while (window.isOpen()) {
			while (window.pollEvent(event)) {
				switch (event.type) {
				case sf::Event::MouseButtonReleased:
					bg.myAudio->addSound(0);
					goto endupWithAudioFadingAway;
					break;
				default:;
				}
			}
			window.clear();
			window.draw(battleOverSp);
			window.draw(overSP1);
			window.draw(overSP);
			window.display();
		}
	}

endupWithAudioFadingAway:
	clock.restart();
	while ((timer = clock.getElapsedTime().asSeconds()) < 0.5f) {
		while (window.pollEvent(event))
			;
		battleOverSp.setColor(sf::Color(255, 255, 255, (int)(255.0f * std::max((0.25f - timer) / 0.25f, 0.0f))));
		bg.myAudio->getBGM()->setVolume(universialVolumn * std::max((0.25f - timer) / 0.25f, 0.0f));
		window.clear();
		window.draw(battleOverSp);
		window.display();
	}

	goto finalEnd;

endupWithoutAudioFadingAway:
	clock.restart();
	while ((timer = clock.getElapsedTime().asSeconds()) < 0.5f) {
		while (window.pollEvent(event))
			;
		battleOverSp.setColor(sf::Color(255, 255, 255, (int)(255.0f * std::max((0.25f - timer) / 0.25f, 0.0f))));
		window.clear();
		window.draw(battleOverSp);
		window.display();
	}

finalEnd:

	gameOver(window, rank, battleOverTX.copyToImage(), bg.myAudio);

	bg.myAudio->reset();

	return ;
}

void gameOver(sf::RenderWindow & window, int rank, sf::Image battleOver, OHMS::AudioCtrl *ac) {
	battleOver = MOHUAAA(battleOver, 30);
	battleOver = MOHUAAA(battleOver, 30);

	sf::Texture lastCloth_TX;
	lastCloth_TX.loadFromImage(battleOver);
	sf::Sprite lastCloth;
	lastCloth.setTexture(lastCloth_TX);

	sf::RectangleShape rect;
	rect.setFillColor(sf::Color(0, 0, 0, 96));
	rect.setSize(sf::Vector2f(winWidth * 1.1f, winHight * 0.35f));
	rect.setPosition(winWidth * -0.05f, winHight * 0.7f);

	sf::Text finText;
	finText.setFont(sp_font);
	finText.setString(L"作战结束");
	finText.setCharacterSize(150);
	finText.setOutlineThickness(4.0f);
	finText.setFillColor(sf::Color::White);
	finText.setOutlineColor(sf::Color::Black);
	finText.setPosition(winWidth * 0.1f, winHight * 0.72f);

	sf::Clock clock;
	float timer;
	clock.restart();
	sf::Event event;
	OHMS::Lihui lihui;

	int chooseOpe = rand() % characterCnt;

	lihui.veryFirstSet("assets/character/" + OHMS::getOperatorStr(chooseOpe) + "/");

	OHMS::AudioCtrl voice(0, 1);
	switch (rank) {
	case 3:
		voice.setSound(0, "assets/character/" + OHMS::getOperatorStr(chooseOpe) + "/sounds/", "finalwin");
		break;
	case 2:
		voice.setSound(0, "assets/character/" + OHMS::getOperatorStr(chooseOpe) + "/sounds/", "finalnormal");
		break;
	case 1:case 0:default:
		voice.setSound(0, "assets/character/" + OHMS::getOperatorStr(chooseOpe) + "/sounds/", "finallose");
	}

	clock.restart();
	while ((timer = clock.getElapsedTime().asSeconds()) < 0.4f) {
		while (window.pollEvent(event))
			;
		globalCloth.setColor(sf::Color(0, 0, 0, 255 - (int)(std::min(timer, 0.25f) / 0.25f * 255.1f)));
		window.clear();
		window.draw(lastCloth);
		lihui.displayIn(window);
		window.draw(rect);
		window.draw(finText);
		window.draw(globalCloth);
		window.display();
	}

	if (rank == 3) {
		ac->changeBGM(1);
	}
	else if (rank == 2) {
		ac->changeBGM(1);
	}
	else {
		ac->changeBGM(3);
	}
	ac->setBGMVolumn(universialVolumn);
	voice.addSound(0);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::MouseButtonReleased:
				ac->addSound(0);
				goto endupShow;
				break;
			default:;
			}
		}
		window.clear();
		window.draw(lastCloth);
		lihui.displayIn(window);
		window.draw(rect);
		window.draw(finText);
		window.display();
	}
endupShow:

	clock.restart();
	while ((timer = clock.getElapsedTime().asSeconds()) < 1.5f) {
		while (window.pollEvent(event))
			;
		rhodesCloth.setColor(sf::Color(255, 255, 255, (int)(std::min(timer, 0.25f) / 0.25f * 255.1f)));
		ac->setBGMVolumn(universialVolumn * std::max((0.25f - timer) / 0.25f, 0.0f));
		window.clear();
		window.draw(lastCloth);
		lihui.displayIn(window);
		window.draw(rhodesCloth);
		window.display();
	}
	return;
}

sf::Image MOHUAAA(sf::Image im0, int radius) {
	sf::Image im, res;
	sf::Vector2u size = im0.getSize();
	im.create(size.x, size.y, sf::Color(0, 0, 0));
	res.create(size.x, size.y, sf::Color(0, 0, 0));
	sf::Color clr[2];
	im = im0;
	int maxlen = std::max(size.x, size.y);
	float* cR = new float[maxlen];
	float* cG = new float[maxlen];
	float* cB = new float[maxlen];
	float* oR = new float[maxlen];
	float* oG = new float[maxlen];
	float* oB = new float[maxlen];
	for (unsigned int i, j = 0, l; j < size.y; ++j) {
		for (i = 0; i < size.x; ++i) {
			clr[0] = im.getPixel(i, j);
			*(oR + i) = (float)((long)clr[0].r * (long)clr[0].r);
			*(oG + i) = (float)((long)clr[0].g * (long)clr[0].g);
			*(oB + i) = (float)((long)clr[0].b * (long)clr[0].b);
			*(cR + i) = 0.0f;
			*(cG + i) = 0.0f;
			*(cB + i) = 0.0f;
		}
		for (int k = 1 - radius; k < radius; ++k) {
			float rate = std::min(0.99999999f, std::max(0.0f, 1.0f - fabsf((float)k) / radius)) / radius;
			for (i = 0; i < size.x; ++i) {
				l = i + k;
				if (l < 0) continue;
				if (l < size.x) {
					*(cR + l) += (*(oR + i) * rate);
					*(cG + l) += (*(oG + i) * rate);
					*(cB + l) += (*(oB + i) * rate);
				}
			}
		}
		for (i = 0; i < size.x; ++i) {
			if (*(cR + i) > 65535.9f) *(cR + i) = 65535.9f;
			if (*(cG + i) > 65535.9f) *(cG + i) = 65535.9f;
			if (*(cB + i) > 65535.9f) *(cB + i) = 65535.9f;
			clr[0].r = (unsigned char) sqrt(*(cR + i));
			clr[0].g = (unsigned char) sqrt(*(cG + i));
			clr[0].b = (unsigned char) sqrt(*(cB + i));
			res.setPixel(i, j, clr[0]);
		}
	}
	im = res;
	for (unsigned int i, j = 0, l; j < size.x; ++j) {
		for (i = 0; i < size.y; ++i) {
			clr[0] = im.getPixel(j, i);
			*(oR + i) = (float)((long)clr[0].r * (long)clr[0].r);
			*(oG + i) = (float)((long)clr[0].g * (long)clr[0].g);
			*(oB + i) = (float)((long)clr[0].b * (long)clr[0].b);
			*(cR + i) = 0.0f;
			*(cG + i) = 0.0f;
			*(cB + i) = 0.0f;
		}
		for (int k = 1 - radius; k < radius; ++k) {
			float rate = std::min(0.99999999f, std::max(0.0f, 1.0f - fabsf((float)k) / radius)) / radius;
			for (i = 0; i < size.y; ++i) {
				l = i + k;
				if (l < 0) continue;
				if (l < size.y) {
					*(cR + l) += (*(oR + i) * rate);
					*(cG + l) += (*(oG + i) * rate);
					*(cB + l) += (*(oB + i) * rate);
				}
			}
		}
		for (i = 0; i < size.y; ++i) {
			if (*(cR + i) > 65535.9f) *(cR + i) = 65535.9f;
			if (*(cG + i) > 65535.9f) *(cG + i) = 65535.9f;
			if (*(cB + i) > 65535.9f) *(cB + i) = 65535.9f;
			clr[0].r = (unsigned char) sqrt(*(cR + i));
			clr[0].g = (unsigned char) sqrt(*(cG + i));
			clr[0].b = (unsigned char) sqrt(*(cB + i));
			res.setPixel(j, i, clr[0]);
		}
	}
	delete[] cR;
	delete[] cG;
	delete[] cB;
	delete[] oR;
	delete[] oG;
	delete[] oB;
	return res;
}

bool tipPop(sf::RenderWindow& window) {
	sf::Image im;
	im.create(winWidth, 400, sf::Color::Black);
	sf::Texture bg;
	bg.loadFromImage(im);
	sf::Sprite sp;
	sp.setTexture(bg);
	sp.setOrigin(winWidth / 2.0f, 200.0f);
	sp.setPosition(winWidth / 2.0f, winHight / 2.0f);
	sf::Text tx;
	tx.setFont(ui_font);
	tx.setFillColor(sf::Color::White);
	tx.setCharacterSize(50);
	sf::FloatRect tmp;
	
	window.draw(sp);

	tx.setString(L"项目未完全实现，只有第三个关卡能够“正常”游玩\n按ESC撤退");
	tmp = tx.getGlobalBounds();
	tx.setOrigin(tmp.width / 2.0f, tmp.height / 2.0f);
	tx.setPosition(winWidth / 2.0f, winHight / 2.0f);

	window.draw(tx);

	window.display();

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::MouseButtonReleased:
				goto OOOOJBK;
				break;
			default:;
			}
		}
	}
OOOOJBK:
	return true;
}
