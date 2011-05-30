<?php

//���_�f�[�^(8���_�xyz�R�v�f)
$vertexes =
    array(
        array(-0.5, 0.5, 0.5),//v0
        array(-0.5, -0.5, 0.5),//v1
        array(0.5, -0.5, 0.5),//v2
        array(0.5, 0.5, 0.5),//v3

        array(-0.5, 0.5, -0.5),//v4
        array(-0.5, -0.5, -0.5),//v5
        array(0.5, -0.5, -0.5),//v6
        array(0.5, 0.5, -0.5)//v7
        );

//------------ �e��ގ��p�����[�^--------------//
//����(������g�U������ʌ���ʒu)
$lightAmb = array(0.0, 0.0, 0.0, 1.0 );
$lightDiff = array(1.0, 1.0, 1.0, 1.0);
$lightSpec = array(1.0, 1.0, 1.0, 1.0);
$lightPos = array(1.0, 1.0, 1.0, 0.0);//���s����

//�ގ�(������g�U������ʌ�����ʎw��)
$goldAmb = array( 0.24725, 0.1995, 0.0745, 1.0 );
$goldDiff = array( 0.75164, 0.60648 , 0.22648, 1.0 );
$goldSpec = array( 0.628281, 0.555802,0.366065,1.0 );
$goldShin =  51.2 ;


//�����̂̕`��
function drawCube()
{
    global $vertexes;
	//top
	glNormal3f(0,1,0);
	glBegin(GL_QUADS);
	glVertex3fv($vertexes[0]);
	glVertex3fv($vertexes[3]);
	glVertex3fv($vertexes[7]);
	glVertex3fv($vertexes[4]);
	glEnd();

	//buttom
	glNormal3f(0,-1,0);
	glBegin(GL_QUADS);
	glVertex3fv($vertexes[5]);
	glVertex3fv($vertexes[6]);
	glVertex3fv($vertexes[2]);
	glVertex3fv($vertexes[1]);
	glEnd();

	//front
	glNormal3f(0,0,1);
	glBegin(GL_QUADS);
	glVertex3fv($vertexes[0]);
	glVertex3fv($vertexes[1]);
	glVertex3fv($vertexes[2]);
	glVertex3fv($vertexes[3]);
	glEnd();

	//back
	glNormal3f(0,0,-1);
	glBegin(GL_QUADS);
	glVertex3fv($vertexes[4]);
	glVertex3fv($vertexes[7]);
	glVertex3fv($vertexes[6]);
	glVertex3fv($vertexes[5]);
	glEnd();

	//left
	glNormal3f(-1,0,0);
	glBegin(GL_QUADS);
	glVertex3fv($vertexes[0]);
	glVertex3fv($vertexes[4]);
	glVertex3fv($vertexes[5]);
	glVertex3fv($vertexes[1]);
	glEnd();

	//right
	glNormal3f(1,0,0);
	glBegin(GL_QUADS);
	glVertex3fv($vertexes[3]);
	glVertex3fv($vertexes[2]);
	glVertex3fv($vertexes[6]);
	glVertex3fv($vertexes[7]);
	glEnd();
}

//--------- �e��R�[���o�b�N�֐�-------//
function display()
{
   	global $lightPos;
    global $goldAmb, $goldDiff, $goldSpec, $goldShin;

	static $angle = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	gluLookAt(0.0, 3.0, 4,0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	//�����̈ʒu�ݒ�
	glLightfv(GL_LIGHT0,GL_POSITION,$lightPos);

	//�ގ��ݒ�ƕ`��
	glPushMatrix();
	glRotatef($angle, 0.0, 1.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,$goldAmb);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,$goldDiff);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,$goldSpec);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,$goldShin);
	drawCube();//�����̂̕`��
	glPopMatrix();

	glutSwapBuffers();
	if(++$angle >= 360){ $angle = 0;}
}

function reshape($w, $h)
{
	glViewport(0,0,$w,$h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0,$w/$h, 0.1, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

function idle()
{
	glutPostRedisplay();
}

//--------- ���̑��e��ݒ�-----------//
function otherInit()
{
	global $lightAmb, $lightDiff, $lightSpec;

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	//�����ݒ�(����g�U����ʂ̂�)
	glLightfv(GL_LIGHT0,GL_AMBIENT,$lightAmb);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,$lightDiff);
	glLightfv(GL_LIGHT0,GL_SPECULAR,$lightSpec);

	//�����ƃ��C�e�B���O�L����
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);//�@���x�N�g���̐��K��
}

//---------- ���C���֐�-------------//
glutInit($argv);
glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
glutInitWindowSize(640,480);
glutCreateWindow('Normal');

//�R�[���o�b�N�֐��o�^
glutDisplayFunc('display');
glutReshapeFunc('reshape');
glutIdleFunc('idle');

otherInit();//���̑��ݒ�

glutMainLoop();

exit (0);
