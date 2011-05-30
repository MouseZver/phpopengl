<?php

//����(������g�U������ʌ���ʒu)
$lightAmb = array(0.0, 0.0, 0.0, 1.0);
$lightDiff = array(1.0, 1.0, 1.0, 1.0);
$lightSpec = array(1.0, 1.0, 1.0, 1.0);
$lightPos = array(1.0, 1.0, 1.0, 0.0);//���s����

//�ގ�(������g�U������ʌ�����ʎw��)
$pearlAmb = array(0.25,0.20725,0.20725,1.0);
$pearlDiff = array(1.0,0.829,0.829,1.0);
$pearlSpec = array(0.296648,0.296648,0.296648,1.0);
$pearlShin =  10.24;

//glu�I�u�W�F�N�g
$quadric = null;

//--------- GLUquadric�I�u�W�F�N�g�̕`��----------------//
function DrawQuadricObj()
{
    global $quadric;
	//�V�����_
	gluQuadricOrientation($quadric,GLU_OUTSIDE);
	gluCylinder($quadric, 0.5, 0.3, 1.0, 20, 15);

	//���
	glPushMatrix();
		glTranslatef(0.0,0.0,1.0);//���������s�ړ�
		gluDisk($quadric, 0, 0.3, 20,10);
	glPopMatrix();

	//���(�@���̌������t�ɂ���)
	gluQuadricOrientation($quadric,GLU_INSIDE);
	gluDisk($quadric, 0, 0.5, 20,10);
}


//--------- �e��R�[���o�b�N�֐�-------//
function display()
{
    global $lightPos;
    global $pearlAmb, $pearlDiff, $pearlSpec, $pearlShin;
    
	static $angle = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	gluLookAt(0.0, 1.5, 4,0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	//�����̈ʒu�ݒ�
	glLightfv(GL_LIGHT0,GL_POSITION,$lightPos);

	//�ގ��ݒ�
	glPushMatrix();
	glRotatef($angle, 0.0, 1.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,$pearlAmb);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,$pearlDiff);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,$pearlSpec);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,$pearlShin);
	DrawQuadricObj();//�`��
	glPopMatrix();

	glutSwapBuffers();
	if (++$angle >= 360) { $angle = 0; }
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
    global $quadric;

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
	glEnable(GL_NORMALIZE);

	//GLUquadric�I�u�W�F�N�g�̐ݒ�
	$quadric =  gluNewQuadric(); //�I�u�W�F�N�g����
	gluQuadricDrawStyle($quadric, GLU_FILL); //�|���S���`��
	gluQuadricNormals($quadric, GLU_SMOOTH); //�V�F�[�f�B���O
}

//---------- ���C���֐�-------------//
glutInit($argv);
glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
glutInitWindowSize(640,480);
glutCreateWindow('GLUquadric Object');

//�R�[���o�b�N�֐��o�^
glutDisplayFunc('display');
glutReshapeFunc('reshape');
glutIdleFunc('idle');

otherInit();//���̑��ݒ�

glutMainLoop();
	
//�s�v�ɂȂ�����j��
gluDeleteQuadric($quadric);//�{���͏I�����������ŋL�q����
exit (0);
