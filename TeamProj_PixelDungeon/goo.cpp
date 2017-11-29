#include "stdafx.h"
#include "goo.h"
#include "Player.h"
#include "UI.h"
#include "Map.h"
#include "itemManager.h"

goo::goo()
{
}


goo::~goo()
{
}

HRESULT goo::init(POINT point, int cog)
{
	//=======================================================================================================================================================
	//												*GOOIMAGE*	
	//=======================================================================================================================================================

	IMAGEMANAGER->addFrameImage("gooMove", "Img\Enemy\mimic\mimic_move.bmp", 64, 64, 2, 2, true, RGB(255, 0, 255));//무브인줄 알았는데 아이들이였다
	IMAGEMANAGER->addFrameImage("gooStay", "Img\Enemy\mimic\mimic_stay.bmp", 160, 64, 5, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("gooDead", "Img\Enemy\mimic\mimic_dead.bmp", 128, 64, 4, 2, true, RGB(255, 0, 255));

	//=======================================================================================================================================================
	//												*GOOINIT*	
	//=======================================================================================================================================================

	//==============================================*LV/STAT SETTING*=============================================================================================================

	_statistics.exp = 10;
	_statistics.lv = 1;  //플레이어 레벨에 비례하여 오릅니다.
	_statistics.maxLv = 30;

	//최대레벨 고정
	if (_statistics.lv >= _statistics.maxLv) _statistics.lv = _statistics.maxLv;

	_statistics.hp = 80 + (_statistics.lv * 4);//레벨에 비례하여 상승합니다.
	_statistics.def = 12;

	_image = IMAGEMANAGER->findImage("gooStay");

	_statistics.avd_lck = 15;
	_statistics.atk_lck = 15;

	//==============================================*CHARACTER INIT*=============================================================================================================

	_hitBox = RectMake(point.x, point.y, TILESIZE, TILESIZE);	//32로 고정을 해줍니다. 혹시 모르니까요.
	_attBox = RectMake(0, 0, 0, 0);								//일단 초기화를 해줍시다.

	_currntHp = _statistics.hp;

	//_hpBar->init(point.x, point.y, TILESIZE, 4);
	//_hpBar->setGauge(_currntHp, _statistics.hp);//hp바를 세팅해줍니다.

	//==============================================*BOOL SETTING*=============================================================================================================

	_myState = ENEMYSTATE_SLEEP;//SLEEP == DISABLE

	_isLive = true;		//살아있습니다.
	_action = false;	//아직 내 턴이 아닙니다.
	_findPlayer = false;	//파인드 플레이어가 플레이어 건드렸는지의 여부입니다.

	//==============================================*etc. SETTING*=============================================================================================================

	//=======================================================================================================================================================
	//*NOTICE
	//*FrameY가 0일시 right
	//*FrameY가 1일시 left
	//=======================================================================================================================================================

	_currentFrameX = 0;	//프레임을 초기화 해줍니다.
	_currentFrameY = 0;	//프레임을 초기화 해줍니다.
	_image->setFrameX(_currentFrameX);	//이미지 프레임을 초기화 해줍니다.
	_image->setFrameY(_currentFrameY);	//이미지 프레임을 초기화 해줍니다.

	_deadAlpha = 255;

	_cog = RectMake(point.x, point.y, cog, cog);

	_point.x = point.x;
	_point.y = point.y;

	_pumpIt = false; //매우 강-력한 공격입니다.
	trunCount = 0; //펌프잇 쓰기위한 턴 카운트, 2가 되면 매우 강력한 공격을 시도 합니다.

	return S_OK;
}

void goo::release()
{
	SAFE_RELEASE(_image);
	SAFE_DELETE(_image);
}

void goo::update()
{
	if (_myState == ENEMYSTATE_SLEEP)
	{
		_statistics.lv = _player->getStat().lv; //플레이어 레벨에 비례하여 오릅니다
		_statistics.hp = 80 + (_statistics.lv * 4);//레벨에 비례하여 상승합니다.

		if (_statistics.lv >= _statistics.maxLv)_statistics.lv = _statistics.maxLv;
	}

	frameUpdate();

	//플레이어 찾는거

	if (_findPlayer == false)
	{
		_findPlayer = true;
		_myState = ENEMYSTATE_IDLE;
		_image = IMAGEMANAGER->findImage("gooStay");
		_action = false;
	}

	//플레이어와 멀어졌다 해서 다시 비활동 상태로 돌아가지는 않는다.
	if (_isLive == true && _findPlayer == true)
	{
		_myState = ENEMYSTATE_IDLE;
		//for (int sightMaxTile = 1; sightMaxTile < 4; sightMaxTile++)
		//{
		//
		//}
		//if (_map->getTile().terrain)
		//{
		//
		//}

		//자신의 턴이 아닐시 이미지는 stay로 설정한다.
		if (_action == false)_image = IMAGEMANAGER->findImage("gooStay");

		//자신의 턴일시 액션을 취한다.
		else if (_action == true)action();
	}

	if (_currntHp <= 0)
	{
		_myState = ENEMYSTATE_DEAD;
		_isLive = false; //움직임을 멈춘다.
		_deadAlpha--;

		//UI에게 사망소식을 알린다.
		//플레이어에게 EXP를 넘긴다.
		//미믹은 랜덤하게 아이템을 뱉는다.

		//itemDrop();
	}

}

void goo::render(POINT camera)
{
	draw(camera);
}

void goo::draw(POINT camera)
{
	RectangleMake(getMemDC(), _point.x + camera.x, _point.y + camera.y, 32, 32);
	//_image->frameRender(getMemDC(), _point.x + camera.x, _point.y + camera.y);
	_image->alphaFrameRender(getMemDC(), _point.x + camera.x, _point.y + camera.y, _currentFrameX, _currentFrameY, _deadAlpha);

}

void goo::frameUpdate()
{
	//============================================*DIRECTION SETTING*===========================================

	if (_findPlayer == true)
	{
		if (_player->getPoint().x > _point.x)	_right = true;
		else									_right = false;
	}

	//==========================================*DIRECTION IMAGE CHANGE*========================================

	if (_right)	_currentFrameY = 0;
	else		_currentFrameY = 1;

	//==============================================*STATE IMAGE CHANGE*========================================

	if (_myState == ENEMYSTATE_MOVE)_image = IMAGEMANAGER->findImage("gooMove");
	else if (_myState == ENEMYSTATE_ATTACK)_image = IMAGEMANAGER->findImage("gooStay");
	if (_myState == ENEMYSTATE_DEAD)_image = IMAGEMANAGER->findImage("goonDead");

	//==============================================*FRAME UPDATE*========================================

	_frameFPS = 10;//프레임 속도 조절용
	_frameTime++;

	if (_frameFPS <= _frameTime)//프레임을 넘긴다
	{
		_currentFrameX++;
		_frameTime = 0;
	}

	//프레임 초기화
	if (_currentFrameX >= _image->getMaxFrameX() && _myState != ENEMYSTATE_DEAD) _currentFrameX = 0;	//죽은상태가 아니면 프레임을 초기화 한다.

	//==============================================*FRAME UPDATE*========================================
}

void goo::action()
{
	//플레이어와 거리를 계산한다, 공격과 이동여부를 정함
	int distanceToPlayer = getDistance(_point.x, _point.y, _player->getPoint().x, _player->getPoint().y) / TILESIZE;

	//1/3 확률로 펌프잇을 사용하기 위한 인트값이다.
	int rnd = RND->getInt(2);

	//두칸 이상 떨어져 있으면 플레이어 위치로 움직입니다.
	if (distanceToPlayer < 2 && _pumpIt == false) move();

	//팜-프 잇이 활성화된 상태에선 사거리가 길기에 세칸 이상 떨어져 있으면 플레이어 위치로 움직인다. 

	else if (distanceToPlayer < 6 && _pumpIt == true)move();

	//아닐시 플레이어 위치로 공격을 시도합니다.

	else if (rnd == 2 && _pumpIt == false) _pumpIt = true;
	else attack();

}

void goo::attack()
{

	_myState = ENEMYSTATE_ATTACK;


	_attBox = RectMake(_player->getPoint().x, _player->getPoint().y, TILESIZE, TILESIZE);

	//턴 카운트를 늘려준다, 펌프잇을 사용하기 위해서

	if (_pumpIt == true) trunCount++;

	//펌프잇을 사용한다.
	else if (_pumpIt == true && trunCount == 2)
	{
		trunCount = 0;
		_statistics.atk_lck = 30;

		//위키에 (17.5 +- 5.32 이라 되어있던데 뭔뜻인지 모르니 걍 넣자
		_statistics.str = RND->getFromIntTo(5 + (_statistics.lv * 1 + 17), 30 + (_statistics.lv * 2 + 17));
		//_player->getDamaged(_statistics.str);
	}
	//평타
	else
	{
		//위키에 (7 +- 2.26 이라 되어있던데 뭔뜻인지 모르니 걍 넣자
		_statistics.str = RND->getFromIntTo(2 + (_statistics.lv * 1+7), 12 + (_statistics.lv * 2+7));
		//_player->getDamaged(_statistics.str);
	}

	_action = false;
}

void goo::move()
{
	//에이스타로 적을 따라 이동합니다.

	_myState = ENEMYSTATE_MOVE;

	//7 8 9
	//4 5 6
	//1 2 3
	//5는 idle



	//A*를 적용하여 '플레이어' 의 '주변 1칸'애 있어야함

	//_map->aStar(_point, _player->getPoint());
	//
	//if (_player->getPoint().y > _point.y)
	//{
	//	_point.y = _point.y + TILESIZE;
	//}

	//좌표값 구하기
	int x = _movePt.x;
	int y = _movePt.y;

	//해당 좌표값에 도착했는지 체크
	if (_point.x == x && _point.y == y)
	{
		_image = IMAGEMANAGER->findImage("brownIdle"); //갈색

		_myState = ENEMYSTATE_IDLE;
		_action = false;
	}
	//좌표값에 도착하지 못함
	else
	{
		//좌우
		if (_point.x > x)	_point.x -= TILESIZE / 8;
		else				_point.x += TILESIZE / 8;

		//상하
		if (_point.y > y)	_point.y -= TILESIZE / 8;
		else				_point.y += TILESIZE / 8;
	}

}

void goo::getDamaged(int damage)
{
	//1/3 확률로 한다. 무슨일이 있어도.
	int rnd = RND->getInt(2);

	_currntHp -= (damage - _statistics.def);

	//디버프에 독인가 그게 없다, 그냥 데미지를 입히는거로 한다.
	if (rnd == 2)
	{
		
	}
}
