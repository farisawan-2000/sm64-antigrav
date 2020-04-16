#define abs(x) ((x) < 0 ? (-x) : (x))
void flagpole_init(void) {

}

void flag_checkpoint_0(void) {
	struct MarioState *m = &gMarioStates[0];

	if (m->interactObj == o) {
		o->oAction = 1;
	}
}

void flag_checkpoint_1(void) {
	struct MarioState *m = &gMarioStates[0];
	f32 topPole = abs(o->hitboxHeight - m->pos[1]);
	if (topPole <= 200.0f && o->oSubAction == 0) {
		o->oSubAction = 1;
		spawn_object(m->marioObj, MODEL_1UP, bhv1Up);
		set_mario_action(m, ACT_TOP_OF_POLE_TRANSITION, 0);
	}
	else if (o->oSubAction == 1) {
		if (is_anim_at_end(m)) {
			set_mario_action(m, ACT_TOP_OF_POLE_TRANSITION, 0);
			o->oAction = 3;
			o->oSubAction = 0;
		}
	}
	else {
		o->oAction = 3;
	}
}
void flag_checkpoint_2(void) {
	struct MarioState *m = &gMarioStates[0];
	if(is_anim_at_end(m)) {
		if (m->actionArg==0)
			o->oAction = 3;
	}
}

void flag_checkpoint_3(void) {
	struct MarioState *m = &gMarioStates[0];
	m->marioObj->oMarioPolePos -= 28.0f;
	if (!(m->action & ACT_WALL_KICK_AIR) && ((m->pos[1] - m->floorHeight) < 20) ) {
		m->faceAngle[1]+=0x8000;
		set_mario_action(m, ACT_WALL_KICK_AIR, 1);
		o->oAction = 4;
	}
}
void flag_checkpoint_4(void) {
	struct MarioState *m = &gMarioStates[0];
	if (m->action & ACT_HOLDING_POLE) {
		m->faceAngle[1]+=0x8000;
		set_mario_action(m, ACT_WALL_KICK_AIR, 1);
		return;
	}
	if (m->action & ACT_WALL_KICK_AIR) {
		o->oAction = 5;
	}
}
void flag_checkpoint_5(void) {
	struct MarioState *m = &gMarioStates[0];
	if (m->interactObj == o) {
		if (m->pos[1] - m->floorHeight < 20.0f) {
			if (m->vel[1] < 0) {
				o->oAction = 0;
				m->interactObj = NULL;
				spawn_object(m->marioObj, MODEL_STAR, bhvStar);
			}
		}
	}
}

void (*sPoleActions[])(void) = {
	flag_checkpoint_0,
	flag_checkpoint_1,
	flag_checkpoint_2,
	flag_checkpoint_3,
	flag_checkpoint_4,
	flag_checkpoint_5,
};

void flagpole_loop(void) {
	cur_obj_call_action_function(sPoleActions);
}
