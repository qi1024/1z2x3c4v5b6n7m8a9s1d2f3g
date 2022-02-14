var inputs=document.getElementsByTagName("input");
var flags=[0,0,0,0,0,0];//最后一位是邮箱验证码发送状态位
var valueRight="";
var timer=null;
var second=null;
var vericode=-1;
var errorTimes=0;

function createCanvas(){
	var code="0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	var canvasStr="";
	valueRight="";//每次都要清空
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

function fronzen(message1,message2){
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

function checkAccount(index){
	var xhr=createXHR();
	xhr.open("POST","user/checkAccount",true);
	xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xhr.responseType="text";
	xhr.onreadystatechange=function(){		//请求完成后的反馈
		if(xhr.readyState==4&&xhr.status==200){
			if(xhr.responseText=="not-exists:account") errorInput(index,"*账号未注册"); 
			else if(xhr.responseText=="exists:account"){
				trueInput(index);
				flags[index]=1;
				inputs[1].focus();
			}
			else window.location.replace("error.html");  //与后天交互不正常，跳转到错误页面
		}
	}
	xhr.send(inputs[0].value);
}


function resetPassword(){		//正式更改密码请求
	var xhr=createXHR();
	xhr.open("POST","/user/resetPassword",true);
	xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xhr.responseType="text";
	xhr.onreadystatechange=function(){
		if(xhr.readyState==4&&xhr.status==200){
			if(xhr.responseText=="updateUser:success"){
				if(confirm("密码修改成功!"))window.location.replace("index.html");
				else window.location.replace("index.html");
			}
			else console.log("xhr.responseText="+xhr.responseText);
			//else window.location.replace("error.html");    		
	 	}
	}
	xhr.send("account="+inputs[0].value+"&password="+inputs[1].value);
}


function checkCode(){
	if(inputs[3].value!=vericode){
		if(++errorTimes==5){
			errorTimes=0;
			errorInput(3,"*输入错误次数超过5次");
			seconds=60;
			fronzen("*验证码输入错误次数超过5次：","后解锁");
			inputs[3].focus();
		}
		else{
			errorInput(3,"*验证码错误"); 
			inputs[3].focus();
		}
	}
	else return resetPassword();
}


function sendCode(){
	var xhr=createXHR();
	xhr.open("POST","other/sendEmailCoded",true);
	xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xhr.responseType="text";
	xhr.onreadystatechange=function(){
		if(xhr.readyState==4&&xhr.status==200){
			flags[5]=0;//复位发送邮箱状态
			var response = JSON.parse(xhr.responseText);
			var result=response.result;
			if(result=="failSend"){
				second=60;
				fronzen("","s后才能获取验证码");
			}
			else if(result=="failBusy") errorInput(3,"服务器繁忙,请稍后在发送验证码");//服务器出现意外
			else if(result=="sendEmailSuccess"){
				seconds=60;
				flags[3]=1;
				vericode=response.vericode;
				fronzen("","s后可重新获取验证码");
				inputs[3].focus();
			}
			else window.location.replace("error.html");  //与后天交互不正常，跳转到错误页面
		}
	}
	xhr.send(inputs[0].value);
	flags[5]=1;//设置邮箱正在发送标志位
}

var functions={
		//检查账号
		"0":function checkUserAccount(){
			var pattern1=/[^@]+@(\w+\.)+\w+/;  //邮箱格式
			//var pattern1=/^\d{11}$/;//手机号格式
			if(inputs[0].value==null||inputs[0].value.trim()==""||inputs[0].value.search(" ")!=-1){
				errorInput(0,"账号不能为空且不能包含空格");
				flags[0]=0;
				inputs[0].focus();
				return false;
			}
			else if(!pattern1.test(inputs[0].value)){
				errorInput(0,"*账号格式不对");
				flags[0]=0;
				inputs[0].focus();
				return false;
			}
			else return checkAccount(0);
		},
		//检查密码
		"1":function checkUserPassword(){
			var pattern2=/^[0-9a-zA-Z]{6,20}$/;	 //密码格式
			if(inputs[1].value==null||inputs[1].value.trim()==""||inputs[1].value.search(" ")!=-1){
				errorInput(1,"密码不能为空且不能包含空格");
				flags[1]=0;
				inputs[1].focus();
				return false;
			}
			else if(!pattern2.test(inputs[1].value)){ 
				errorInput(1,"*密码格式位6-20位不包含中文和空格的字符"); 
				flags[1]=0;
				inputs[1].focus();
				return false;
			}
			else{
				trueInput(1);
				flags[1]=1;
				inputs[2].focus();
			}
		},
		//检查确认密码
		"2":function checkconfirmPassword(){
			if(inputs[2].value!=inputs[1].value){
				errorInput(2,"两次密码不一致!");
				flags[2]=0;
				inputs[2].focus();
				return false;
			}
			else{
				trueInput(2);
				flags[2]=1;
				document.getElementsByClassName("grayout")[0].style.display="block";
				inputs[4].focus();
			}
		},
		//检查验证码
		"3":function checkVericode(){
			if(flags[3]==0){
				document.getElementsByClassName("grayout")[0].style.display="block";
				inputs[4].focus();
			}
			else return true;
		},
		//检查邮箱验证码交给后端检查
		"4":function checkEmailCode(){
			if(inputs[3].value==null||inputs[3].value.trim()==""){
				alert("*验证码不能为空");
				inputs[3].focus();
				return false;
			}
			else if(inputs[3].value.length!=6){
				alert("*验证码错误！");
				inputs[3].focus();
				return false;
			}
			return checkCode();
		}
}


function checkInput(len){//要检查的函数最大索引,从0开始算
	if(flags[5]){//先判断验证码是否启动
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

function addEvent(){
	//点击确认更改时触发
	document.getElementsByClassName("confirmReset")[0].onclick=function(){
		checkInput(4);
	}
	
	//点击获取验证码时触发
	document.getElementById("getCode").onclick=function(){
		if(!checkInput(3)) return false;
		else if(timer!=null){alert("验证码冷却中！请勿重复操作！");return false;}
		else sendCode(3);
	}
	for(var i=0;i<inputs.length-1;++i) inputs[i].onchange=functions[i];//非邮箱验证码不用触发
	
	//点击关闭弹窗时触发
	document.getElementsByClassName("close")[0].onclick=function(){
		document.getElementsByClassName("grayout")[0].style.display="none";
		createCanvas();
	}
	
	//点击确认前端时触发
	document.getElementsByClassName("vericodeConfirm")[0].onclick=function(){
		var inputVericode=inputs[4].value.toLocaleLowerCase();
		if(inputVericode!=valueRight.toLocaleLowerCase()){
			flags[3]=0;
			document.getElementsByClassName("vericode-error")[0].innerHTML="*验证码错误";
			return false;
		}
		else{
			flags[3]=1;
			sendCode();
			document.getElementsByClassName("close")[0].click();//通过触发关闭事件关闭验证还能刷新哦!
			document.getElementsByTagName("body")[0].removeChild(document.getElementsByClassName("grayout")[0]);
		}
	}
	
	//点击刷新验证码时触发
	document.getElementsByClassName("refresh")[0].onclick=function(){
		createCanvas();
	}
	
	//前端验证码文本框遇到回车时触发
	document.getElementsByName("userVericode")[0].onkeydown=function(e){
		var e=e||window.event;
		if(e.keyCode==13)
			document.getElementsByClassName("vericodeConfirm")[0].click();
	}
	
	//当在表单中按下回车时触发
	document.getElementsByTagName("form")[0].onkeydown=function(e){
		var e=e||window.event;
		if(e.keyCode==13){
			document.getElementsByClassName("confirmReset")[0].click();
		}
	}
	
	//当点击返回登录时触发
	document.getElementById("backToLogin").onclick=function(){
		window.location.replace("/login.html");
	}
	//当点击现在注册时触发
	document.getElementById("registerNow").onclick=function(){
		window.location.replace("/register.html");
	}
	
}

window.onload=function(){
	addEvent();
	createCanvas();

}
