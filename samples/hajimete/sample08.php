<?php

/*	�O���ϐ�	*/
$status = 0;//�{�^���̏��

/* �e��R�[���o�b�N�֐�*/
//�f�B�X�v���C
function display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

//�}�E�X(�N���b�N)
function mouse($button, $state, $x, $y)
{
    global $status;
	if($button == GLUT_LEFT_BUTTON && $state == GLUT_DOWN)
	{
		printf("���{�^���������܂���@ (%d,%d)\n",$x,$y);
		$status = 1;
	}else if($button == GLUT_LEFT_BUTTON && $state == GLUT_UP)
	{
		printf("���{�^���𗣂��܂���@ (%d,%d)\n",$x,$y);
		$status = 0;
	}
}

//�h���b�O
function dragMotion($x, $y)
{
    global $status;
	if($status == 1){
		printf("DragMotion @ (%d,%d)\n",$x,$y);
	}
}

//�p�b�V�u
function passiveMotion($x, $y)
{
	printf("PassiveMotion @ (%d,%d)\n",$x,$y);
}

/*	���C���֐�	*/
glutInit($argv);
glutCreateWindow('Mouse Callback');

//�e��R�[���o�b�N�֐��̓o�^
glutDisplayFunc('display');
glutMouseFunc('mouse');
glutMotionFunc('dragMotion');
glutPassiveMotionFunc('passiveMotion');

glClearColor(1.0, 1.0, 1.0, 1.0);

glutMainLoop();

exit (0);
