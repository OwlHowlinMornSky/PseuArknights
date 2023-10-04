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
#pragma once

#include "gamecore.h"

#define OHMS_GAME_STATEMACHINE_DEFAULT 0
#define OHMS_GAME_STATEMACHINE_IDLE 1
#define OHMS_GAME_STATEMACHINE_ATTACK 2
#define OHMS_GAME_STATEMACHINE_DIE 3
#define OHMS_GAME_STATEMACHINE_SET 4

namespace OHMS {
	class EntityState;
	class EntityStateMachine;

	class EntityState_DEFAULT;
	class EntityState_IDLE;
	class EntityState_ATTACK;
	class EntityState_DIE;
	class EntityState_SET;

	class EntityState {
		friend class EntityStateMachine;
	protected:
		OHMS::EntityMain* myEntity;
		OHMS::EntityStateMachine* myMachine;
		void veryFirstSet(OHMS::EntityMain* theEntity, OHMS::EntityStateMachine* theMachine);
	public:
		EntityState();
		virtual void firstIn();
		virtual void update();
		virtual void checkOut();
	};

	class EntityStateMachine {
		friend class EntityState;
		friend class EntityState_DEFAULT;
		friend class EntityState_IDLE;
		friend class EntityState_ATTACK;
		friend class EntityState_DIE;
		friend class EntityState_SET;
	private:
		OHMS::EntityState** states;
		OHMS::EntityMain* myEntity;
		unsigned int flag, num;
	public:
		EntityStateMachine();
		void veryFirstSet(OHMS::EntityMain* theEntity, bool isEnemy);
		void changeState(unsigned int dist);
		void update();
	};

	class EntityState_DEFAULT : public EntityState {
	public:
		void firstIn();
		void update();
		void checkOut();
	};
	class EntityState_IDLE : public EntityState {
		bool animeIsMove = false;
	public:
		void firstIn();
		void update();
		void checkOut();
	};
	class EntityState_ATTACK : public EntityState {
	public:
		void firstIn();
		void update();
		void checkOut();
	};
	class EntityState_DIE : public EntityState {
	public:
		void firstIn();
		void update();
		void checkOut();
	};
	class EntityState_SET : public EntityState {
	public:
		void firstIn();
		void update();
		void checkOut();
	};
}
