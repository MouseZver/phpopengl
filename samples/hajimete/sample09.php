<?php

/* �Ƽ拾����Хå��ؿ�*/
//�ǥ����ץ쥤
function display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

//�����ܡ��ɽ���(�������Ȥ�)
function keyboard($key, $x, $y)
{
	if (is_string($key)) { $key = ord($key); }
	if($key == 27)//[ESC]�����ΤȤ�
	{
		echo 'Exit...'.PHP_EOL;
		exit(0);
	}

	printf("%c ��������ޤ���@ (%d, %d)\n",$key,$x,$y);
}

//�����ܡ��ɽ���(Υ�����Ȥ�)
function keyboardUp($key, $x, $y)
{
	if (is_string($key)) { $key = ord($key); }
	printf("%c ��Υ����ޤ���@ (%d, %d)\n",$key,$x,$y);
}

//�ü쥭���򲡤����Ȥ�
function specialKey($key, $x, $y)
{
	if (is_string($key)) { $key = ord($key); }
	switch($key)
	{
	case GLUT_KEY_LEFT:
		printf("[��]��������ޤ���@ (%d, %d)\n",$x,$y);
		break;

	case GLUT_KEY_UP:
		printf("[��]��������ޤ���@ (%d, %d)\n",$x,$y);
		break;

	case GLUT_KEY_RIGHT:
		printf("[��]��������ޤ���@ (%d, %d)\n",$x,$y);
		break;

	case GLUT_KEY_DOWN:
		printf("[��]��������ޤ���@ (%d, %d)\n",$x,$y);
		break;
	}
}

//�ü쥭����Υ�����Ȥ�
function specialUpKey($key, $x, $y)
{
	if (is_string($key)) { $key = ord($key); }
	switch($key)
	{
	case GLUT_KEY_LEFT:
		printf("[��]��Υ��ޤ���@ (%d, %d)\n",$x,$y);
		break;

	case GLUT_KEY_UP:
		printf("[��]��Υ��ޤ���@ (%d, %d)\n",$x,$y);
		break;

	case GLUT_KEY_RIGHT:
		printf("[��]��Υ��ޤ���@ (%d, %d)\n",$x,$y);
		break;

	case GLUT_KEY_DOWN:
		printf("[��]��Υ��ޤ���@ (%d, %d)\n",$x,$y);
		break;
	}
}

/*	�ᥤ��ؿ�	*/
glutInit($argv);
glutCreateWindow('Key Callback');

//�Ƽ拾����Хå��ؿ�����Ͽ
glutDisplayFunc('display');

//�̾省��
glutKeyboardFunc('keyboard');
glutKeyboardUpFunc('keyboardUp');
//�ü쥭��
glutSpecialFunc('specialKey');
glutSpecialUpFunc('specialUpKey');
glutIgnoreKeyRepeat(GL_TRUE);//��������ԡ���̵��

glClearColor(1.0, 1.0, 1.0, 1.0);

glutMainLoop();

exit (0);

