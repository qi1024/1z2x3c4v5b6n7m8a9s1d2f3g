var valueRight="";
var seconds=null;
var timer=null;
var errorTimes=0;
function fronzen(message1,message2){		//冻结注册功能，防止恶意注册
	clearInterval(timer);
    timer=setInterval(function countDown(){
		if((seconds--)>0){
			document.getElementById("userPassword-error").innerHTML=message1+seconds+message2;
			return ;
		}
		else{
			clearInterval(timer);
			timer=null;
			document.getElementById("userPassword-error").innerHTML="";
			return ;
		}
	},1000);
}


//创建验证码
function createCanvas(){
	var code="0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	var canvasStr="";
	valueRight="";//每次都清空;
	for(var i=0,len=code.length;i<6;++i){
		var text=code[Math.floor(Math.random()*len)];
		canvasStr += text + ' ';
	    valueRight += text;
	}
	var myCanvas=document.getElementsByClassName("canvas")[0];
	var ctx = myCanvas.getContext('2d');
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

function login(){			//登录函数
	var xhr=createXHR();
	var account=document.getElementsByName("userAccount")[0].value.trim();
	var password=document.getElementsByName("userPassword")[0].value.trim();
	xhr.open("POST","/user/login",true);
	xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xhr.responseType="text";
	xhr.onreadystatechange=function(){
		if(xhr.readyState==4&&xhr.status==200){
			if(xhr.responseText=="login:fail"){
				if(++errorTimes==5){
					errorTimes=0;
					seconds=60;
					fronzen("*密码错误次数超过5次，","后重新登录");
				}
				else{
					document.getElementById("userPassword-error").innerHTML="*账号或密码错误";
					document.getElementsByName("userPassword")[0].focus();
				}
			}
			else if(xhr.responseText=="login:success"){
				document.cookie="account="+account+"&password="+password;
				window.location.replace("/index.html");
			}
			else window.location.replace("error.html");		
		}
	}
	xhr.send("account="+account+"&password="+password);
}

function checkInput(){
	var vericode=document.getElementsByClassName("vericode")[0].value.trim();
	var account=document.getElementsByName("userAccount")[0].value.trim();
	var password=document.getElementsByName("userPassword")[0].value.trim();
	var pattern1=/[^@]+@(\w+\.)+\w+/;  //邮箱格式
	//var pattern1=/^\d{11}$/;//手机格式
	var pattern2=/^[0-9a-zA-Z]{6,20}$/;	 //密码格式
	if(!pattern1.test(account)){
		document.getElementById("userAccount-error").innerHTML="*账号格式不对";
		document.getElementsByName("userAccount")[0].focus();
		return false;
	}
	else if(password.length<6||password.length>20||!pattern2.test(password)){
		document.getElementById("userPassword-error").innerHTML="*密码格式不对";
		document.getElementsByName("userPassword")[0].focus();
		return false;
	}
	else if(vericode==null||vericode==""||vericode.toLowerCase()!=valueRight.toLowerCase()){		//初步检查账号
		document.getElementsByClassName("vericode-error")[0].innerHTML="*验证码错误";
		document.getElementsByClassName("vericode")[0].focus();
		return false;
	}
	else if(timer!=null) return false;
	return login();
}

function addEvent(){
	//点击确认登录时触发
	document.getElementsByClassName("confirmLogin")[0].onclick=function(){
		document.getElementById("userAccount-error").innerHTML="";
		document.getElementById("userPassword-error").innerHTML="";
		document.getElementsByClassName("vericode-error")[0].innerHTML="";
		checkInput();
	}

	//点击刷新验证码时触发
	document.getElementsByClassName("refresh")[0].onclick=function(){
		createCanvas();
	}
	//点击现在注册时触发
	document.getElementById("registerNow").onclick=function(){
		window.location.replace("/register.html");
	}
	
	//点击忘记密码时触发
	document.getElementById("forgetPassword").onclick=function(){
		window.location.replace("/ResetPassword.html");
	}
	
	//按下回车时触发
	document.getElementsByClassName("inputArea")[0].onkeydown=function(e){
		var e=e||window.event;
		if(e.keyCode==13)document.getElementsByClassName("confirmLogin")[0].click();
	}
}

window.onload=function(){
	createCanvas();
	addEvent();
}