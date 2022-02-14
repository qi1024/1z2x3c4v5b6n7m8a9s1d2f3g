/**
 * 以下是天速晚报网站各大网页所要用到的工具包
 */
//适用于Ajax请求中的创建XMLRequest对象
function createXHR(){
	if(window.XMLHttpRequest){//非IE浏览器
		return new XMLHttpRequest();
	}
	else if(window.ActiveXObject){//IE浏览器出现的两种可能
		try{
			return new ActiveXObject("Msxml2.XMLHTTP");
		}catch(e){
			try{
				return new ActiveXObject("Microsoft.XMLHTTP");
			}catch(e){alert("出现未知错误,无法创建XMLHttpRequest对象,无法发送Ajax!");}
		}
	}
}

//下面是防止闭包的模板
/*
 function test(){
 	var arr=[];
 	for(var i=0;i<10;++i){
 		(function(j){
 		arr[j]=function(){document.write(j);}
 		})
 	}
 }
 
 */

//对标签的浅复制,前复制只复制一层
function shallowClone(origin,target){
	var target=target||{};//如果target为null,新建对象位{}
	for(var prop in orign) target[prop]=origin[prop];
	return target;
}

//对标签的深层复制,可以复制对象中的对象
function deepClone(origin,target){
	var target=target||{};//如果target为null,新建对象位{}
	var toStr=Object.prototype.toString,arrStr="[object Array]";
	for(var prop in origin){
		if(origin.hasOwnProperty(prop)){
			if(origin[prop]!=="null"&&typeof(origin[prop])=="object"){
				if(toStr.call(origin[prop])==arrStr) target[prop]=[];
				else target[prop]={}
				deepClone(origin[prop],target[prop]);
			}
			else target[prop]=origin[prop];
		}
	}
	return target;
}

//自定义更完善的判断数据类型的函数type
function type(target){
	var template={
			"[object Array]":  "array",
			"[object Object]": "object",
			"[object Number]": "number-object",
			"[object Boolean]": "boolean-object",
			"[object String]": "String-object"
	}
	if(target==null) return "null"; //注意这里是字符串的"null"
	else if(typeof(target)=="object"){
		var str=Object.prototype.toString.call(target);
		return template[str];
	}
	else return typeof(target)//剩下的是基本类型,可以直接用自带的typeof();
}

/*原生js没有数组去重的方法,可以巧妙实现
 * 在原型链上实现
 */

Array.prototype.unique=function(){
	var temp={}, arr=[],len=this.length;
	for(var i=0;i<len;++i){
		if(!temp[this[i]]){
			temp[this[i]]="abc";
			arr.push(this[i]);
		}
	}
	return arr;
}

//添加,移除,判断是否含有某个类名
function hasClass(className,data,command){//command通过什么方式获取判断对象
	switch(command){
		case "id":
			return document.getElementById(data).className.indexOf(className)>0;
			break;
		case "class":
			return document.getElementsByClassName(data)[0].className.indexOf(className)>0;
			break;
		case "name":
			return document.getElementsByName(data)[0].className.indexOf(className)>0;
			break;
		case "tagName":
			return document.getElementsByTagName(data)[0].className.indexOf(className)>0;
			break;
		default:
			return false;
			break;
	}
}

function addClass(className,data,command){//command通过什么方式获取判断对象,command口令的意思
	switch(command){
		case "id":
			if(!hasClass(className,data,command))document.getElementById(data).className+=" "+className;
			break;
		case "class":
			if(!hasClass(className,data,command))document.getElementsByClassName(data)[0].className+=" "+className;
			break;
		case "name":
			if(!hasClass(className,data,command)) document.getElementsByName(data)[0].className+=" "+className;
			break;
		case "tagName":
			if(!hasClass(className,data,command)) document.getElementsByTagName(data)[0].className+=" "+className;
			break;
		default:
			break;
	}
}

function removeClass(className,data,command){
	switch(command){//删除class的话,好像不用判断该类是否存在
	case "id":
		document.getElementById(data).className=document.getElementById(data).className.replace(" "+className ,"");
		break;
	case "class":
		document.getElementsByClassName(data)[0].className=
		document.getElementsByClassName(data)[0].className.replace(" "+className,"");
		break;
	case "name":
		document.getElementsByName(data)[0].className=
		document.getElementsByName(data)[0].className.replace(" "+className,"");
		break;
	case "tagName":
		 document.getElementsByTagName(data)[0].className=
		 document.getElementsByTagName(data)[0].className.replace(" "+className,"");
		break;
	default:
		break;
	}
}

function replaceClass(sourceClass,targetClass,data,command){
	switch(command){//替换class的话,好像也不用判断该类是否存在,既然是替换也不用考虑空格;
	case "id":
		document.getElementById(data).className=
		document.getElementById(data).className.replace(sourceClass,targetClass);
		break;
	case "class":
		document.getElementsByClassName(data)[0].className=
		document.getElementsByClassName(data)[0].className.replace(sourceClass,targetClass);
		break;
	case "name":
		document.getElementsByName(data)[0].className=
		document.getElementsByName(data)[0].className.replace(sourceClass,targetClass);
		break;
	case "tagName":
		document.getElementsByTagName(data)[0].className=
		 document.getElementsByTagName(data)[0].className.replace(sourceClass,targetClass);
		break;
	default:
		break;
	}
}
