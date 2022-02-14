var inputs=document.getElementsByTagName("input");
var flags=[0,0,0,0,0,0,0];//最后一位是邮箱验证码发送状态位
var valueRight="";
var timer=null;
var seconds=null;
var vericode=9999999;
var errorTimes=0;

function errorInput(index,message){
	document.getElementById(inputs[index].name+"-true").style.display="none";
	document.getElementById(inputs[index].name+"-false").style.display="inline-block";
	document.getElementById(inputs[index].name+"-error").innerHTML=message;
}

function trueInput(index){		
	document.getElementById(inputs[index].name+"-false").style.display="none";
	document.getElementById(inputs[index].name+"-true").style.display="inline-block";
	document.getElementById(inputs[index].name+"-error").innerHTML="";
}

function fronzen(message1,message2){		//冻结注册功能，防止恶意注册
	clearInterval(timer);
    timer=setInterval(function countDown(){
		if((seconds--)>0){
			document.getElementById("confirmPassword-error").innerHTML=message1+seconds+message2;
			return ;
		}
		else{
			clearInterval(timer);
			timer=null;
			document.getElementById("confirmPassword-error").innerHTML="";
			return ;
		}
	},1000);
}

function checkNickname(index){
	var xhr=createXHR();
	xhr.open("POST","user/checkNickname",true);
	xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xhr.responseType="text";
	xhr.onreadystatechange=function(){		//请求完成后的反馈
		if(xhr.readyState==4&&xhr.status==200){
			flags[6]=0;//复位发送邮箱状态
			if(xhr.responseText=="exists:nickname") errorInput(index,"*用户名已存在"); 
			else if(xhr.responseText=="not-exists:nickname"){
				trueInput(index);
				flags[index]=1;
				inputs[1].focus()
			}
			else window.location.replace("error.html");   //与后台交互不正常，跳转到错误页面
		}
	}
	xhr.send(inputs[0].value);
}

function checkAccount(index){
	var xhr=createXHR();
	xhr.open("POST","user/checkAccount",true);
	xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xhr.responseType="text";
	xhr.onreadystatechange=function(){		//请求完成后的反馈
		if(xhr.readyState==4&&xhr.status==200){
			if(xhr.responseText=="exists:account") errorInput(index,"*账号已注册"); 
			else if(xhr.responseText=="not-exists:account"){
				trueInput(index);
				flags[index]=1;
				inputs[2].focus();
			}
			else window.location.replace("error.html");  //与后天交互不正常，跳转到错误页面
		}
	}
	xhr.send(inputs[1].value);
}

function sendCode(index){
	var xhr=createXHR();
	xhr.open("POST","other/sendEmailCode",true);
	xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xhr.responseType="text";
	xhr.onreadystatechange=function(){
		if(xhr.readyState==4&&xhr.status==200){
			flags[6]=0;//复位发送邮箱状;
			var response = JSON.parse(xhr.responseText);
			var result=response.result;
			if(result=="failSend"){
				second=60;
				fronzen("","s后才能获取验证码");
			}
			else if(result=="failBusy") errorInput(index,"服务器繁忙,请稍后在发送验证码");//服务器出现意外
			else if(result=="sendEmailSuccess"){
				seconds=60;
				vericode=response.vericode;
				flags[index]=1;
				fronzen("","s后可重新获取验证码");
				inputs[4].focus();
			}
			else window.location.replace("error.html");  //与后天交互不正常，跳转到错误页面
		}
	}
	xhr.send(inputs[1].value);
	flags[6]=1;
}

function Register(){				//正式注册的请求
	var xhr=createXHR();
	xhr.open("POST","/user/enroll",true);
	xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xhr.responseType="text";
	xhr.onreadystatechange=function(){
		if(xhr.readyState==4&&xhr.status==200){
			if(xhr.responseText=="insertUser:success"){
				if(window.confirm("注册成功！"))window.location.replace("index.html");
				else window.location.replace("index.html");
			}
			else {
				alert("注册失败");
				window.location.replace("error.html");    		//与后台约定的交互不正常，跳转到错误页面
			}
	  }
	}
	xhr.send("nickname="+inputs[0].value+"&account="+inputs[1].value+"&password="+inputs[2].value);
}

function checkCode(){
	if(inputs[4].value!=vericode){
		if(++errorTimes==5){
			errorTimes=0;
			errorInput(4,"*输入错误次数超过5次");
			seconds=60;
			fronzen("*验证码输入错误次数超过5次：","后解锁");
			inputs[4].focus();
		}
		else{
			errorInput(4,"*验证码错误"); 
			inputs[4].focus();
		}
	}
	else return Register();
}

var functions={
		//检查用户名
		"0":function checkUserName(){
			if(inputs[0].value==null||inputs[0].value.trim()==""||inputs[0].value.search(" ")!=-1){
				errorInput(0,"用户名不能为空且不能包含空格");
				flags[0]=0;
				inputs[0].focus();
				return false;
			}
			else if(inputs[0].value.length<2||inputs[0].value.length>15){
				errorInput(0,"*用户名的格式是2-15位字符");
				flags[0]=0;
				inputs[0].focus();
				return false;
			}
			else return checkNickname(0);
		},
		//检查账号
		"1":function checkUserAccount(){
			var pattern1=/[^@]+@(\w+\.)+\w+/;  //邮箱格式
			//var pattern1=/^\d{11}$/;			//手机号格式
			if(inputs[1].value==null||inputs[1].value.trim()==""||inputs[1].value.search(" ")!=-1){
				errorInput(1,"账号不能为空且不能包含空格");
				flags[1]=0;
				inputs[1].focus();
				return false;
			}
			else if(!pattern1.test(inputs[1].value)){
				errorInput(1,"*邮箱号格式不对");
				flags[1]=0;
				inputs[1].focus();
				return false;
			}
			else checkAccount(1);
		},
		//检查密码
		"2":function checkUserPassword(){
			var pattern2=/^[0-9a-zA-Z]{6,20}$/;	 //密码格式
			if(inputs[2].value==null||inputs[2].value.trim()==""||inputs[2].value.search(" ")!=-1){
				errorInput(2,"密码不能为空且不能包含空格");
				flags[2]=0;
				inputs[2].focus();
				return false;
			}
			else if(!pattern2.test(inputs[2].value)){ 
				errorInput(2,"*密码格式位6-20位不包含中文和空格的字符"); 
				flags[2]=0;
				inputs[2].focus();
				return false;
			}
			else{
				trueInput(2);
				flags[2]=1;
				inputs[3].focus();
			}
		},
		//检查确认密码
		"3":function checkconfirmPassword(){
			if(inputs[3].value==null||inputs[2].value.trim()==""||inputs[3].value.search(" ")!=-1){
				errorInput(3,"密码不能为空且不能包含空格");
				flags[3]=0;
				inputs[3].focus();
				return false;
			}
			else if(inputs[3].value!=inputs[2].value){
				errorInput(3,"两次密码不一致!");
				flags[3]=0;
				inputs[3].focus();
				return false;
			}
			else{
				trueInput(3);
				flags[3]=1;
				document.getElementsByClassName("grayout")[0].style.display="block";
				inputs[5].focus();
			}
		},
		//检查前端验证码
		"4":function checkVericode(){
			if(flags[4]==0){
				document.getElementsByClassName("grayout")[0].style.display="block";
				inputs[5].focus();
			}
			else return true;
		},
		//检查邮箱验证码
		"5":function checkEmailCode(){
			if(inputs[4].value==null||inputs[4].value.trim()==""){
				alert("*验证码不能为空");
				inputs[4].focus();
				return false;
			}
			else if(inputs[4].value.length!=6){
				alert("*验证码错误！");
				inputs[4].focus();
				return false;
			}
			else if(inputs[4].value.indexOf(" ")!=-1){
				alert("*验证码错误！");
				inputs[4].focus();
				return false;
			}
			return checkCode();
		}
}


function checkInput(len){//要检查的函数最大索引,从0开始算
	if(flags[6]){//先判断验证码是否启动
		alert("验证码启动中,请勿重复操作!");
		return false;
	}
	for(var i=0;i<=len;++i){//最后一位状态标志位不考虑
		if(flags[i]==0){
			functions[i]();
			return false;
		}
	}
	return true;
}


function createCanvas(){
	var code="0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	var canvasStr="";
	valueRight="";//每次都要清空
	for(var i=0,len=code.length;i<6;++i){
		var text=code[Math.floor(Math.random()*len)];
		canvasStr += text + " ";
	    valueRight += text;
	}
	var myCanvas=document.getElementsByClassName("canvas")[0];
	var ctx = myCanvas.getContext("2d");
	//画线
	ctx.beginPath();
    ctx.clearRect(0,0,myCanvas.width,myCanvas.height);
    ctx.lineWidth = 15;
    ctx.strokeStyle = '#ccc';
    ctx.moveTo(Math.floor(Math.random()*50),Math.floor(Math.random()*80));
    ctx.lineTo(250 + Math.floor(Math.random()*30),Math.floor(Math.random()*80));
    ctx.stroke();
    ctx.globalCompositeOperation="lighter";
    ctx.closePath();
    // 将文字写入canvas画布中
    ctx.save();
    ctx.beginPath();
    var x = myCanvas.width / 2;
    ctx.textAlign = 'center';
    ctx.fillStyle = '#ddd';
    ctx.font = '46px Roboto Slab';
    ctx.setTransform(1, -0.12, 0.2, 1, 0, 12);
    ctx.fillText(canvasStr, x, 60);
    ctx.restore();
}

function rainCodeBegin(){
	var canvas=document.getElementById("raincode");
	var context=canvas.getContext("2d");
	var width=window.innerWidth;
	var height=window.innerHeight;
	canvas.width=width;
	canvas.height=height;
	var fontSize=16;
	var colunms=Math.floor(width/fontSize);
	var drops=[];
	for(var i=0;i<colunms;++i) drops[i]=1;
	var str="JavaScript function(){}";
	function draw(){
		context.fillStyle="rgba(0,0,0,0.05)";
		context.fillRect(0,0,width,height);
		context.font="700 "+fontSize+"px 微软雅黑";
		context.fillStyle="#00cc33";
		for(var i=0;i<colunms;++i){
			var index=Math.floor(Math.random()*str.length);
			var x=i*fontSize;
			var y=drops[i]*fontSize;
			context.fillText(str[index],x,y);
			if(y>=canvas.height&&Math.random()>0.99)
				drops[i]=0;
			++drops[i];
		}
	};
	draw();
	timer=setInterval(draw,20);
}

function addEvent(){
	//点击注册按钮时触发
	document.getElementsByClassName("submitButton")[0].onclick=function(){
		if(!checkInput(5)) return false;
		return checkCode();
	}
	
	//点击获取验证码时触发
	document.getElementsByClassName("getCodeA")[0].onclick=function(){
		if(!checkInput(4)) return false;
		else if(timer!=null){alert("验证码冷却中！请勿重复操作！");return false;}
		else sendCode(4);
		
	}
	
	//每一个输入框失去焦点时触发
	for(var i=0;i<inputs.length-1;++i) inputs[i].onchange=functions[i];
	
	//点击关闭按钮触发
	document.getElementsByClassName("close")[0].onclick=function(){
		document.getElementsByClassName("grayout")[0].style.display="none";
		createCanvas();
	}
	
	//点击前端验证码确认时触发
	document.getElementsByClassName("vericodeConfirm")[0].onclick=function(){
		var inputVericode=inputs[5].value.toLocaleLowerCase();
		if(inputVericode!=valueRight.toLocaleLowerCase()){
			flags[4]=0;
			document.getElementsByClassName("vericode-error")[0].innerHTML="*验证码错误";
			return false;
		}
		else{
			flags[4]=1;
			sendCode(4);
			document.getElementsByClassName("close")[0].click();//通过触发关闭事件关闭验证还能刷新哦!
			document.getElementsByTagName("body")[0].removeChild(document.getElementsByClassName("grayout")[0]);
		}
	}
	
	//点击刷新前端验证码时触发
	document.getElementsByClassName("refresh")[0].onclick=function(){
		createCanvas();
	}
	
	//前端验证码遇到回车时触发
	inputs[5].onkeydown=function(event){
		if(flags[4]==1)return ;
		else if(event.keyCode==13)document.getElementsByClassName("vericodeConfirm")[0].click();
	}
	
	//表单填写遇到回车时触发
	document.getElementsByTagName("form")[0].onkeydown=function(event){
		if(event.keyCode==13)document.getElementsByClassName("submitButton")[0].click();
	}
	
	//点击忘记密码时触发
	document.getElementsByClassName("forgetPassword")[0].onclick=function(){
		window.location.replace("/ResetPassword.html");
	}
	
	//点击现在登录时触发
	document.getElementsByClassName("loginNow")[0].onclick=function(){
		window.location.replace("/login.html");
	}
	
	//点击取消开场动画
	document.getElementById("raincode").onclick=function(){
		document.getElementsByTagName("body")[0].removeChild(document.getElementById("raincode"));
		clearInterval(timer);
		inputs[0].focus();
	}
}

window.onload=function(){
	addEvent();
	createCanvas();
	rainCodeBegin();
}
