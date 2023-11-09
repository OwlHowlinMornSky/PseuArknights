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
#include "gamestatemachine.h"

OHMS::EntityState::EntityState() {
	return;
}

void OHMS::EntityState::veryFirstSet(OHMS::EntityMain* theEntity, OHMS::EntityStateMachine* theMachine) {
	this->myEntity = theEntity;
	this->myMachine = theMachine;
	return;
}

void OHMS::EntityState::firstIn() {
	return;
}

void OHMS::EntityState::update() {
	return this->checkOut();
}

void OHMS::EntityState::checkOut() {
	return;
}

void OHMS::EntityStateMachine::changeState(unsigned int dist) {
	this->flag = dist;
	return (*(this->states + this->flag))->firstIn();
}

OHMS::EntityStateMachine::EntityStateMachine() {
	this->states = nullptr;
	this->myEntity = nullptr;
	this->flag = 0;
	this->num = 0;
	return;
}

void OHMS::EntityStateMachine::veryFirstSet(OHMS::EntityMain* theEntity, bool isEnemy) {
	this->myEntity = theEntity;
	this->num = 5;
	this->states = new OHMS::EntityState*[this->num];
	*(this->states) = new OHMS::EntityState_DEFAULT;
	(*(this->states))->veryFirstSet(this->myEntity, this);
	*(this->states + 1) = new OHMS::EntityState_IDLE;
	(*(this->states + 1))->veryFirstSet(this->myEntity, this);
	*(this->states + 2) = new OHMS::EntityState_ATTACK;
	(*(this->states + 2))->veryFirstSet(this->myEntity, this);
	*(this->states + 3) = new OHMS::EntityState_DIE;
	(*(this->states + 3))->veryFirstSet(this->myEntity, this);
	*(this->states + 4) = new OHMS::EntityState_SET;
	(*(this->states + 4))->veryFirstSet(this->myEntity, this);
	return;
}

void OHMS::EntityStateMachine::update() {
	return (* (this->states + this->flag))->update();
}

void OHMS::EntityState_DEFAULT::firstIn() {
	this->myEntity->changeAnime(ENTITY_ANIMATION_DEFAULT);
	return;
}

void OHMS::EntityState_DEFAULT::update() {
	printf_s("Entity %p is in wrong state!\n", this->myEntity);
	return this->checkOut();
}

void OHMS::EntityState_DEFAULT::checkOut() {
	if (this->myEntity->isDied()) {
		return this->myMachine->changeState(OHMS_GAME_STATEMACHINE_DIE);
	}
	return;
}

void OHMS::EntityState_IDLE::firstIn() {
	this->animeIsMove = false;
	this->myEntity->changeAnime(ENTITY_ANIMATION_IDLE);
	return;
}

void OHMS::EntityState_IDLE::update() {
	if (!this->animeIsMove && this->myEntity->isEnemy && !this->myEntity->mustStop) {
		this->myEntity->changeAnime(ENTITY_ANIMATION_MOVE);
		this->animeIsMove = true;
	}
	else if (this->myEntity->isEnemy && this->myEntity->mustStop) {
		this->myEntity->changeAnime(ENTITY_ANIMATION_IDLE);
		this->animeIsMove = false;
	}
	return this->checkOut();
}

void OHMS::EntityState_IDLE::checkOut() {
	if (this->myEntity->isDied()) {
		return this->myMachine->changeState(OHMS_GAME_STATEMACHINE_DIE);
	}
	if (this->myEntity->canAttack()) {
		return this->myMachine->changeState(OHMS_GAME_STATEMACHINE_ATTACK);
	}
	return;
}

void OHMS::EntityState_ATTACK::firstIn() {
	this->myEntity->changeAnime(ENTITY_ANIMATION_ATTACK);
	this->myEntity->arm->getGoal();
	this->myEntity->arm->aNewRound();
	return;
}

void OHMS::EntityState_ATTACK::update() {
	this->myEntity->arm->update();
	return this->checkOut();
}

void OHMS::EntityState_ATTACK::checkOut() {
	if (this->myEntity->isDied()) {
		return this->myMachine->changeState(OHMS_GAME_STATEMACHINE_DIE);
	}
	if (this->myEntity->getAnime()->playOver()) {
		if (this->myEntity->canAttack()) {
			return this->myMachine->changeState(OHMS_GAME_STATEMACHINE_ATTACK);
		}
		else {
			return this->myMachine->changeState(OHMS_GAME_STATEMACHINE_IDLE);
		}
	}
	if (!this->myEntity->arm->check()) {
		return this->myMachine->changeState(OHMS_GAME_STATEMACHINE_IDLE);
	}
	return;
}

void OHMS::EntityState_DIE::firstIn() {
	this->myEntity->normal = false;
	this->myEntity->changeAnime(ENTITY_ANIMATION_DIE);
	return;
}

void OHMS::EntityState_DIE::update() {
	return this->checkOut();
}

void OHMS::EntityState_DIE::checkOut() {
	if (this->myEntity->getAnime()->playOver()) {
		this->myEntity->Leave = true;
		return this->myMachine->changeState(OHMS_GAME_STATEMACHINE_DEFAULT);
	}
	return;
}

void OHMS::EntityState_SET::firstIn() {
	this->myEntity->changeAnime(ENTITY_ANIMATION_SET);
	return;
}

void OHMS::EntityState_SET::update() {
	return this->checkOut();
}

void OHMS::EntityState_SET::checkOut() {
	if (this->myEntity->getAnime()->playOver()) {
		this->myEntity->normal = true;
		return this->myMachine->changeState(OHMS_GAME_STATEMACHINE_IDLE);
	}
	return;
}
