<?php
require("funcs.php");
require("reg.inc.php");
html_init("gb2312");

if ($loginok != 1)
	html_nologin();
else
{
	$new_reg_mail = $_POST["newemail"];
	
	if(!strcmp($currentuser["userid"],"guest"))
	{
		html_error_quit("���ȵ�¼!");
		exit();
	}
	//��鼤����
	$activation = "";
	$userid = $currentuser["userid"];
	$ret = bbs_getactivation($userid,$activation);
	
	if($ret == -1 || $ret == -10)
		html_error_quit("ϵͳ��������ϵ����Ա!");
		
	if($ret == 0)
	{
		if(bbs_reg_haveactivated($activation))
			html_error_quit("�����ʻ��Ѽ���");	
		$reg_email = bbs_reg_getactivationemail($activation);
	}
	else
		$reg_email = "";
	
?>
<body>
<br /><br />
<?php
		if($new_reg_mail)
		{
			$new_activation = bbs_reg_newactivation(bbs_create_activation(),$new_reg_mail);
			$ret = bbs_setactivation($userid,$new_activation);
			if($ret != 0)
				html_error_quit("ϵͳ����");
			
			$mailbody="
<?xml version=\"1.0\" encoding=\"gb2312\">
<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">
<html>
<body><P class=MsoNormal><FONT size=2><SPAN 
style=\"FONT-FAMILY: ����; mso-ascii-font-family: 'Times New Roman'; mso-hansi-font-family: 'Times New Roman'\">  " . $realname  . "��ӭ������</SPAN><SPAN 
style=\"FONT-FAMILY: ����; mso-ascii-font-family: 'Times New Roman'; mso-hansi-font-family: 'Times New Roman'\">" . BBS_FULL_NAME . "��</SPAN></FONT></P>
<P class=MsoNormal><FONT size=2><SPAN 
style=\"FONT-FAMILY: ����; mso-ascii-font-family: 'Times New Roman'; mso-hansi-font-family: 'Times New Roman'\">����ע����Ϣ�ǣ�</SPAN></FONT></P>
<P class=MsoNormal><FONT size=2><SPAN 
style=\"FONT-FAMILY: ����; mso-ascii-font-family: 'Times New Roman'; mso-hansi-font-family: 'Times New Roman'\">�û�����" . $userid . "</SPAN></FONT></P>
<P class=MsoNormal><FONT size=2><SPAN 
style=\"FONT-FAMILY: ����; mso-ascii-font-family: 'Times New Roman'; mso-hansi-font-family: 'Times New Roman'\">�ǳƣ�" . $nickname . "<SPAN></FONT></P>
<P class=MsoNormal><FONT size=2><SPAN 
style=\"FONT-FAMILY: ����; mso-ascii-font-family: 'Times New Roman'; mso-hansi-font-family: 'Times New Roman'\">���룺" . $password . "</SPAN></FONT></P>
<P class=MsoNormal><FONT size=2><SPAN lang=EN-US>email</SPAN><SPAN 
style=\"FONT-FAMILY: ����; mso-ascii-font-family: 'Times New Roman'; mso-hansi-font-family: 'Times New Roman'\">��" . $reg_email . "</SPAN></FONT></P>
<P class=MsoNormal><FONT size=2><A 
href=\"https://www.smth.edu.cn/bbsact.php?userid=".$userid."&acode=".$activation."<SPAN 
style=\"FONT-FAMILY: ����; mso-ascii-font-family: 'Times New Roman'; mso-hansi-font-family: 'Times New Roman'\">������Ｄ������" . BBS_FULL_NAME . "�����ʺ�</SPAN>
<br /><br />
<P class=MsoNormal><FONT size=2><SPAN 
style=\"FONT-FAMILY: ����; mso-ascii-font-family: 'Times New Roman'; mso-hansi-font-family: 'Times New Roman'\"></SPAN></FONT></P>
</body>
</html>
";
/* To send HTML mail, you can set the Content-type header. */
			$headers  = "MIME-Version: 1.0\r\n";
			$headers .= "Content-type: text/html; charset=gb2312\r\n";
			
			/* additional headers */
			$headers .= "From: BBSˮľ�廪վ <https://www.smth.edu.cn>\r\n";
			
			if(!mail($new_reg_mail, "welcome to " . BBS_FULL_NAME, $mailbody,$headers))
			{		
?>
<p align="center">
�����뷢��ʧ�ܣ���������������ע��Email��
</p><p align="center">
<a href="/bbssendacode.php">[���·��ͼ�����]</a>
</p>
<?php
			}
			else
			{
?>
<p align="center">
�����뷢�ͳɹ���������ż������������ʺš�
</p><p align="center">
<a href-"/bbsfillform.html">[��дע�ᵥ]</a>
</p>
<?php
			}
		}
		else
		{
?>
<form action="bbssendacode.php" method="post">
Emai��ַ��
<input type="text" name="newemail" size="20" maxlength="100" value="<?php echo $reg_email; ?>" />
<input type="submit" value="���ͼ�����" />
</form>
<?php
		}
		html_normal_quit();
	}
?>