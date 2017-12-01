#include "stdafx.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Map.h"
#include "UI.h"

EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

HRESULT EnemyManager::init()
{
	_enemyTurn = false;
	_actionCount = 0;

	//setEnemy(PointMake(12, 12), 2);
	//setEnemy(PointMake(TILESIZE*14, TILESIZE*13), 0);
	//setEnemy(PointMake(13, 13), 2);

	return S_OK;
}

void EnemyManager::release()
{

}

void EnemyManager::update()
{
	for (auto i : _vEnemy)
	{
		i->update();
	}


	//차례가 몬스터에게 넘겨질 경우 && 몬스터의 수가 하나 이상 있을 경우
	if (_enemyTurn && _vEnemy.size() > 0)
	{
		//몬스터 행동
		action();
	}
	//그냥 차례만 넘겨받은 경우
	else if (_enemyTurn)
	{
		//몬스터 차례 false 대입
		_enemyTurn = false;
		//플레이어 차례 true 대입
		_player->setTurn(true);
	}
}
void EnemyManager::action()
{

	if (_actionCount == 0)
	{
		//첫번째 몬스터에게 턴 넘김
		_vEnemy[_actionCount]->setAction(true);
		//다음차례 몬스터 번호 저장
		_actionCount++;
	}
	else if (_vEnemy[_actionCount - 1]->getAction() == false && _actionCount < _vEnemy.size())	//전 차례의 몬스터 행동이 끝났으면
	{
		//다음차례 몬스터에게 턴 넘김
		_vEnemy[_actionCount]->setAction(true);
		//다음차례 몬스터 번호 저장
		_actionCount++;
	}

	bool allEnemyTurnOver = true;	//모든 적이 행동 마쳤는지 확인하기 위한 함수
	for (auto i : _vEnemy)
	{
		//차례를 마치지 않은 적이 있으면 allEnemyTurnOver = false
		if (i->getAction()) allEnemyTurnOver = false;
	}

	//모든 적이 행동을 마쳤으면
	if (allEnemyTurnOver)
	{
		//몬스터 차례 false 대입
		_enemyTurn = false;
		_actionCount = 0;
		//플레이어 차례 true 대입
		_player->setTurn(true);
	}
}
void EnemyManager::render()
{

}

//그릴 때	x좌표에 camera.x 만큼
//			y좌표에 camera.y 만큼 더해주기!!!!
void EnemyManager::render(POINT camera)
{	
	for (auto i : _vEnemy)
	{		
		i->render(camera);
	}
}
void EnemyManager::draw(POINT camera)
{

}

void EnemyManager::setEnemy(POINT point, int type)
{
	switch (type)
	{
		case 0:
		{

			Rat* temp = new Rat;
			temp->init(point);
			temp->setPlayerAddressLink(_player);
			temp->setItemManagerAddressLink(_im);
			temp->setUiAddressLink(_ui);
			temp->setMapAddressLink(_map);
			_vEnemy.push_back(temp);
			break;

		//쥐, Rat

		break;
		}
		case 1:
		{

			Mimic* temp = new Mimic;
			temp->init(point,4);
			temp->setPlayerAddressLink(_player);
			temp->setItemManagerAddressLink(_im);
			temp->setUiAddressLink(_ui);
			temp->setMapAddressLink(_map);
			_vEnemy.push_back(temp);

			//미믹, Mimic
			break;
		}
		case 2:
		{
			//파리, Swarm
			Swarm* temp = new Swarm;
			temp->init(point);
			temp->setEm(this);
			temp->setPlayerAddressLink(_player);
			temp->setItemManagerAddressLink(_im);
			temp->setUiAddressLink(_ui);
			temp->setMapAddressLink(_map);
			_vEnemy.push_back(temp);
			break;
		}
		case 3:
		{
			//놀, Gnoll
			Gnoll* temp = new Gnoll;
			temp->init(point);
			temp->setPlayerAddressLink(_player);
			temp->setItemManagerAddressLink(_im);
			temp->setUiAddressLink(_ui);
			temp->setMapAddressLink(_map);
			_vEnemy.push_back(temp);
			break;
		}
		case 4:
		{

			goo* temp = new goo;
			temp->init(point,8);
			temp->setPlayerAddressLink(_player);
			temp->setItemManagerAddressLink(_im);
			temp->setUiAddressLink(_ui);
			temp->setMapAddressLink(_map);

			break;

		}
		case 5:
		{
			//
			break;
		}
		default:
		{
			//
			break;
		}
	}
}

void EnemyManager::setSwarm(POINT point, int hp)
{
	//파리, Swarm
	Swarm* temp = new Swarm;
	temp->init(point, hp);
	temp->setEm(this);
	temp->setPlayerAddressLink(_player);
	temp->setItemManagerAddressLink(_im);
	temp->setUiAddressLink(_ui);
	temp->setMapAddressLink(_map);
	_vEnemy.push_back(temp);
}