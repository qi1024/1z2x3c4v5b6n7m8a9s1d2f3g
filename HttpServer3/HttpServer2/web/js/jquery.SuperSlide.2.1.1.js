
!function(a){a.fn.slide=function(b){return a.fn.slide.defaults={type:"slide",effect:"fade",autoPlay:!1,delayTime:500,interTime:2500,triggerTime:150,defaultIndex:0,titCell:".hd li",mainCell:".bd",targetCell:null,trigger:"mouseover",scroll:1,vis:1,titOnClassName:"on",autoPage:!1,prevCell:".prev",nextCell:".next",pageStateCell:".pageState",opp:!1,pnLoop:!0,easing:"swing",startFun:null,endFun:null,switchLoad:null,playStateCell:".playState",mouseOverStop:!0,defaultPlay:!0,returnDefault:!1},this.each(function(){var c=a.extend({},a.fn.slide.defaults,b),d=a(this),e=c.effect,f=a(c.prevCell,d),g=a(c.nextCell,d),h=a(c.pageStateCell,d),i=a(c.playStateCell,d),j=a(c.titCell,d),k=j.size(),l=a(c.mainCell,d),m=l.children().size(),n=c.switchLoad,o=a(c.targetCell,d),p=parseInt(c.defaultIndex),q=parseInt(c.delayTime),r=parseInt(c.interTime);parseInt(c.triggerTime);var Q,t=parseInt(c.scroll),u=parseInt(c.vis),v="false"==c.autoPlay||0==c.autoPlay?!1:!0,w="false"==c.opp||0==c.opp?!1:!0,x="false"==c.autoPage||0==c.autoPage?!1:!0,y="false"==c.pnLoop||0==c.pnLoop?!1:!0,z="false"==c.mouseOverStop||0==c.mouseOverStop?!1:!0,A="false"==c.defaultPlay||0==c.defaultPlay?!1:!0,B="false"==c.returnDefault||0==c.returnDefault?!1:!0,C=0,D=0,E=0,F=0,G=c.easing,H=null,I=null,J=null,K=c.titOnClassName,L=j.index(d.find("."+K)),M=p=-1==L?p:L,N=p,O=p,P=m>=u?0!=m%t?m%t:t:0,R="leftMarquee"==e||"topMarquee"==e?!0:!1,S=function(){a.isFunction(c.startFun)&&c.startFun(p,k,d,a(c.titCell,d),l,o,f,g)},T=function(){a.isFunction(c.endFun)&&c.endFun(p,k,d,a(c.titCell,d),l,o,f,g)},U=function(){j.removeClass(K),A&&j.eq(N).addClass(K)};if("menu"==c.type)return A&&j.removeClass(K).eq(p).addClass(K),j.hover(function(){Q=a(this).find(c.targetCell);var b=j.index(a(this));I=setTimeout(function(){switch(p=b,j.removeClass(K).eq(p).addClass(K),S(),e){case"fade":Q.stop(!0,!0).animate({opacity:"show"},q,G,T);break;case"slideDown":Q.stop(!0,!0).animate({height:"show"},q,G,T)}},c.triggerTime)},function(){switch(clearTimeout(I),e){case"fade":Q.animate({opacity:"hide"},q,G);break;case"slideDown":Q.animate({height:"hide"},q,G)}}),B&&d.hover(function(){clearTimeout(J)},function(){J=setTimeout(U,q)}),void 0;if(0==k&&(k=m),R&&(k=2),x){if(m>=u)if("leftLoop"==e||"topLoop"==e)k=0!=m%t?(0^m/t)+1:m/t;else{var V=m-u;k=1+parseInt(0!=V%t?V/t+1:V/t),0>=k&&(k=1)}else k=1;j.html("");var W="";if(1==c.autoPage||"true"==c.autoPage)for(var X=0;k>X;X++)W+="<li>"+(X+1)+"</li>";else for(var X=0;k>X;X++)W+=c.autoPage.replace("$",X+1);j.html(W);var j=j.children()}if(m>=u){l.children().each(function(){a(this).width()>E&&(E=a(this).width(),D=a(this).outerWidth(!0)),a(this).height()>F&&(F=a(this).height(),C=a(this).outerHeight(!0))});var Y=l.children(),Z=function(){for(var a=0;u>a;a++)Y.eq(a).clone().addClass("clone").appendTo(l);for(var a=0;P>a;a++)Y.eq(m-a-1).clone().addClass("clone").prependTo(l)};switch(e){case"fold":l.css({position:"relative",width:D,height:C}).children().css({position:"absolute",width:E,left:0,top:0,display:"none"});break;case"top":l.wrap('<div class="tempWrap" style="overflow:hidden; position:relative; height:'+u*C+'px"></div>').css({top:-(p*t)*C,position:"relative",padding:"0",margin:"0"}).children().css({height:F});break;case"left":l.wrap('<div class="tempWrap" style="overflow:hidden; position:relative; width:'+u*D+'px"></div>').css({width:m*D,left:-(p*t)*D,position:"relative",overflow:"hidden",padding:"0",margin:"0"}).children().css({"float":"left",width:E});break;case"leftLoop":case"leftMarquee":Z(),l.wrap('<div class="tempWrap" style="overflow:hidden; position:relative; width:'+u*D+'px"></div>').css({width:(m+u+P)*D,position:"relative",overflow:"hidden",padding:"0",margin:"0",left:-(P+p*t)*D}).children().css({"float":"left",width:E});break;case"topLoop":case"topMarquee":Z(),l.wrap('<div class="tempWrap" style="overflow:hidden; position:relative; height:'+u*C+'px"></div>').css({height:(m+u+P)*C,position:"relative",padding:"0",margin:"0",top:-(P+p*t)*C}).children().css({height:F})}}var $=function(a){var b=a*t;return a==k?b=m:-1==a&&0!=m%t&&(b=-m%t),b},_=function(b){var c=function(c){for(var d=c;u+c>d;d++)b.eq(d).find("img["+n+"]").each(function(){var b=a(this);if(b.attr("src",b.attr(n)).removeAttr(n),l.find(".clone")[0])for(var c=l.children(),d=0;d<c.size();d++)c.eq(d).find("img["+n+"]").each(function(){a(this).attr(n)==b.attr("src")&&a(this).attr("src",a(this).attr(n)).removeAttr(n)})})};switch(e){case"fade":case"fold":case"top":case"left":case"slideDown":c(p*t);break;case"leftLoop":case"topLoop":c(P+$(O));break;case"leftMarquee":case"topMarquee":var d="leftMarquee"==e?l.css("left").replace("px",""):l.css("top").replace("px",""),f="leftMarquee"==e?D:C,g=P;if(0!=d%f){var h=Math.abs(0^d/f);g=1==p?P+h:P+h-1}c(g)}},ab=function(a){if(!A||M!=p||a||R){if(R?p>=1?p=1:0>=p&&(p=0):(O=p,p>=k?p=0:0>p&&(p=k-1)),S(),null!=n&&_(l.children()),o[0]&&(Q=o.eq(p),null!=n&&_(o),"slideDown"==e?(o.not(Q).stop(!0,!0).slideUp(q),Q.slideDown(q,G,function(){l[0]||T()})):(o.not(Q).stop(!0,!0).hide(),Q.animate({opacity:"show"},q,function(){l[0]||T()}))),m>=u)switch(e){case"fade":l.children().stop(!0,!0).eq(p).animate({opacity:"show"},q,G,function(){T()}).siblings().hide();break;case"fold":l.children().stop(!0,!0).eq(p).animate({opacity:"show"},q,G,function(){T()}).siblings().animate({opacity:"hide"},q,G);break;case"top":l.stop(!0,!1).animate({top:-p*t*C},q,G,function(){T()});break;case"left":l.stop(!0,!1).animate({left:-p*t*D},q,G,function(){T()});break;case"leftLoop":var b=O;l.stop(!0,!0).animate({left:-($(O)+P)*D},q,G,function(){-1>=b?l.css("left",-(P+(k-1)*t)*D):b>=k&&l.css("left",-P*D),T()});break;case"topLoop":var b=O;l.stop(!0,!0).animate({top:-($(O)+P)*C},q,G,function(){-1>=b?l.css("top",-(P+(k-1)*t)*C):b>=k&&l.css("top",-P*C),T()});break;case"leftMarquee":var c=l.css("left").replace("px","");0==p?l.animate({left:++c},0,function(){l.css("left").replace("px","")>=0&&l.css("left",-m*D)}):l.animate({left:--c},0,function(){l.css("left").replace("px","")<=-(m+P)*D&&l.css("left",-P*D)});break;case"topMarquee":var d=l.css("top").replace("px","");0==p?l.animate({top:++d},0,function(){l.css("top").replace("px","")>=0&&l.css("top",-m*C)}):l.animate({top:--d},0,function(){l.css("top").replace("px","")<=-(m+P)*C&&l.css("top",-P*C)})}j.removeClass(K).eq(p).addClass(K),M=p,y||(g.removeClass("nextStop"),f.removeClass("prevStop"),0==p&&f.addClass("prevStop"),p==k-1&&g.addClass("nextStop")),h.html("<span>"+(p+1)+"</span>/"+k)}};A&&ab(!0),B&&d.hover(function(){clearTimeout(J)},function(){J=setTimeout(function(){p=N,A?ab():"slideDown"==e?Q.slideUp(q,U):Q.animate({opacity:"hide"},q,U),M=p},300)});var bb=function(a){H=setInterval(function(){w?p--:p++,ab()},a?a:r)},cb=function(a){H=setInterval(ab,a?a:r)},db=function(){z||(clearInterval(H),bb())},eb=function(){(y||p!=k-1)&&(p++,ab(),R||db())},fb=function(){(y||0!=p)&&(p--,ab(),R||db())},gb=function(){clearInterval(H),R?cb():bb(),i.removeClass("pauseState")},hb=function(){clearInterval(H),i.addClass("pauseState")};if(v?R?(w?p--:p++,cb(),z&&l.hover(hb,gb)):(bb(),z&&d.hover(hb,gb)):(R&&(w?p--:p++),i.addClass("pauseState")),i.click(function(){i.hasClass("pauseState")?gb():hb()}),"mouseover"==c.trigger?j.hover(function(){var a=j.index(this);I=setTimeout(function(){p=a,ab(),db()},c.triggerTime)},function(){clearTimeout(I)}):j.click(function(){p=j.index(this),ab(),db()}),R){if(g.mousedown(eb),f.mousedown(fb),y){var ib,jb=function(){ib=setTimeout(function(){clearInterval(H),cb(0^r/10)},150)},kb=function(){clearTimeout(ib),clearInterval(H),cb()};g.mousedown(jb),g.mouseup(kb),f.mousedown(jb),f.mouseup(kb)}"mouseover"==c.trigger&&(g.hover(eb,function(){}),f.hover(fb,function(){}))}else g.click(eb),f.click(fb)})}}(jQuery),jQuery.easing.jswing=jQuery.easing.swing,jQuery.extend(jQuery.easing,{def:"easeOutQuad",swing:function(a,b,c,d,e){return jQuery.easing[jQuery.easing.def](a,b,c,d,e)},easeInQuad:function(a,b,c,d,e){return d*(b/=e)*b+c},easeOutQuad:function(a,b,c,d,e){return-d*(b/=e)*(b-2)+c},easeInOutQuad:function(a,b,c,d,e){return(b/=e/2)<1?d/2*b*b+c:-d/2*(--b*(b-2)-1)+c},easeInCubic:function(a,b,c,d,e){return d*(b/=e)*b*b+c},easeOutCubic:function(a,b,c,d,e){return d*((b=b/e-1)*b*b+1)+c},easeInOutCubic:function(a,b,c,d,e){return(b/=e/2)<1?d/2*b*b*b+c:d/2*((b-=2)*b*b+2)+c},easeInQuart:function(a,b,c,d,e){return d*(b/=e)*b*b*b+c},easeOutQuart:function(a,b,c,d,e){return-d*((b=b/e-1)*b*b*b-1)+c},easeInOutQuart:function(a,b,c,d,e){return(b/=e/2)<1?d/2*b*b*b*b+c:-d/2*((b-=2)*b*b*b-2)+c},easeInQuint:function(a,b,c,d,e){return d*(b/=e)*b*b*b*b+c},easeOutQuint:function(a,b,c,d,e){return d*((b=b/e-1)*b*b*b*b+1)+c},easeInOutQuint:function(a,b,c,d,e){return(b/=e/2)<1?d/2*b*b*b*b*b+c:d/2*((b-=2)*b*b*b*b+2)+c},easeInSine:function(a,b,c,d,e){return-d*Math.cos(b/e*(Math.PI/2))+d+c},easeOutSine:function(a,b,c,d,e){return d*Math.sin(b/e*(Math.PI/2))+c},easeInOutSine:function(a,b,c,d,e){return-d/2*(Math.cos(Math.PI*b/e)-1)+c},easeInExpo:function(a,b,c,d,e){return 0==b?c:d*Math.pow(2,10*(b/e-1))+c},easeOutExpo:function(a,b,c,d,e){return b==e?c+d:d*(-Math.pow(2,-10*b/e)+1)+c},easeInOutExpo:function(a,b,c,d,e){return 0==b?c:b==e?c+d:(b/=e/2)<1?d/2*Math.pow(2,10*(b-1))+c:d/2*(-Math.pow(2,-10*--b)+2)+c},easeInCirc:function(a,b,c,d,e){return-d*(Math.sqrt(1-(b/=e)*b)-1)+c},easeOutCirc:function(a,b,c,d,e){return d*Math.sqrt(1-(b=b/e-1)*b)+c},easeInOutCirc:function(a,b,c,d,e){return(b/=e/2)<1?-d/2*(Math.sqrt(1-b*b)-1)+c:d/2*(Math.sqrt(1-(b-=2)*b)+1)+c},easeInElastic:function(a,b,c,d,e){var f=1.70158,g=0,h=d;if(0==b)return c;if(1==(b/=e))return c+d;if(g||(g=.3*e),h<Math.abs(d)){h=d;var f=g/4}else var f=g/(2*Math.PI)*Math.asin(d/h);return-(h*Math.pow(2,10*(b-=1))*Math.sin((b*e-f)*2*Math.PI/g))+c},easeOutElastic:function(a,b,c,d,e){var f=1.70158,g=0,h=d;if(0==b)return c;if(1==(b/=e))return c+d;if(g||(g=.3*e),h<Math.abs(d)){h=d;var f=g/4}else var f=g/(2*Math.PI)*Math.asin(d/h);return h*Math.pow(2,-10*b)*Math.sin((b*e-f)*2*Math.PI/g)+d+c},easeInOutElastic:function(a,b,c,d,e){var f=1.70158,g=0,h=d;if(0==b)return c;if(2==(b/=e/2))return c+d;if(g||(g=e*.3*1.5),h<Math.abs(d)){h=d;var f=g/4}else var f=g/(2*Math.PI)*Math.asin(d/h);return 1>b?-.5*h*Math.pow(2,10*(b-=1))*Math.sin((b*e-f)*2*Math.PI/g)+c:.5*h*Math.pow(2,-10*(b-=1))*Math.sin((b*e-f)*2*Math.PI/g)+d+c},easeInBack:function(a,b,c,d,e,f){return void 0==f&&(f=1.70158),d*(b/=e)*b*((f+1)*b-f)+c},easeOutBack:function(a,b,c,d,e,f){return void 0==f&&(f=1.70158),d*((b=b/e-1)*b*((f+1)*b+f)+1)+c},easeInOutBack:function(a,b,c,d,e,f){return void 0==f&&(f=1.70158),(b/=e/2)<1?d/2*b*b*(((f*=1.525)+1)*b-f)+c:d/2*((b-=2)*b*(((f*=1.525)+1)*b+f)+2)+c},easeInBounce:function(a,b,c,d,e){return d-jQuery.easing.easeOutBounce(a,e-b,0,d,e)+c},easeOutBounce:function(a,b,c,d,e){return(b/=e)<1/2.75?d*7.5625*b*b+c:2/2.75>b?d*(7.5625*(b-=1.5/2.75)*b+.75)+c:2.5/2.75>b?d*(7.5625*(b-=2.25/2.75)*b+.9375)+c:d*(7.5625*(b-=2.625/2.75)*b+.984375)+c},easeInOutBounce:function(a,b,c,d,e){return e/2>b?.5*jQuery.easing.easeInBounce(a,2*b,0,d,e)+c:.5*jQuery.easing.easeOutBounce(a,2*b-e,0,d,e)+.5*d+c}});

var account="";
var password="";
var ws=null;
var targetTr=null;

function stampToTime(datetime){
    var date = new Date(datetime);//时间戳为秒需*1000，时间戳为毫秒的话不需乘1000
    var year = date.getFullYear(),
        month = ("0" + (date.getMonth() + 1)).slice(-2),
        sdate = ("0" + date.getDate()).slice(-2),
        hour = ("0" + date.getHours()).slice(-2),
        minute = ("0" + date.getMinutes()).slice(-2),
        second = ("0" + date.getSeconds()).slice(-2);
    // 拼接
    var result = year + "-"+ month +"-"+ sdate +" "+ hour +":"+ minute +":" + second;
    return result;
}

function addEvent(){

    document.getElementById("upload").onclick=function(){
        document.getElementById("uploadLi").click();
        var tr=document.createElement("tr");
        var td0=document.createElement("td");
        var file=document.createElement("input");
        file.type="file";
        file.accept="*/*";
        file.onchange=function(){
            var td1=document.createElement("td");
            var td2=document.createElement("td");
            var td3=document.createElement("td");
            var td4=document.createElement("td");
            td0.style.display="none";
            td0.appendChild(file);
            td1.innerHTML=file.files[0].name;
            td2.innerHTML="等待上传";
            td3.innerHTML=file.files[0].size;
            td4.innerHTML="<a>上传</a>"
            tr.appendChild(td0);
            tr.appendChild(td1);
            tr.appendChild(td2);
            tr.appendChild(td3);
            tr.appendChild(td4);
            document.getElementById("uploadDetail").appendChild(tr);
        }
        file.click();
    }

    document.getElementById("download").onclick=function(){
        var fileCode= document.getElementById("fileCode").value;
        if(fileCode==null || fileCode.trim()=="")return alert("文件提取码为空！");
        else if(fileCode.trim().length!=20)return alert("文件提取码为20位英文字母加数字！");
        $("#downloadLi").click();
        $.ajax({
                url: '/getFile?fileCode='+fileCode,
                type: 'POST',
                cache: false,
                data: "",
                dataType : "json",
                processData: false,
                contentType: false,
                success: function(responseText){
                    var tr=document.createElement("tr");
                    var codeTd=document.createElement("td");
                    var nameTd=document.createElement("td");
                    var uploadTd=document.createElement("td");
                    var deadlineTd=document.createElement("td");
                    var sizeTd=document.createElement("td");
                    var downtd=document.createElement("td");

                    downtd.innerHTML="<a class='pay-now lg-layer'>下载</a>";
                    codeTd.innerHTML=responseText.fileCode;
                    nameTd.innerHTML=responseText.fileName;
                    uploadTd.innerHTML=responseText.uploadTime;
                    var timestamp =(new Date(responseText.uploadTime)).getTime();
                    deadlineTd.innerHTML=stampToTime(timestamp+24*60*60*1000);
                    sizeTd.innerHTML=responseText.fileSize;
                    tr.appendChild(codeTd);
                    tr.appendChild(nameTd);
                    tr.appendChild(uploadTd);
                    tr.appendChild(deadlineTd);
                    tr.appendChild(sizeTd);
                    tr.appendChild(downtd);
                    document.getElementById("downloadDetail").appendChild(tr);
                },
                error:function(errmess){
                    alert("文件提取码错误或文件已过期!");
                    return ;
                }

            });
    }

    document.getElementById("downloadDetail").onclick=function(e){
        var target=e.target||window.event.target;
        if(target.tagName=="A") {
            var tr=target.parentNode.parentNode;
     		var fileCode=tr.getElementsByTagName("TD")[0].innerHTML;
            var fileSize=tr.getElementsByTagName("TD")[4].innerHTML;
            if(fileSize>=4*1024*1024*1024){
                if(window.confirm("该文件属于超大文件，请耐心等待！"))
                    var win=window.open("/download?fileCode="+fileCode);
            }
            else var win=window.open("/download?fileCode="+fileCode);
        }
    }

    function upload(e){
        var target=e.target||window.event.target;
        if(target.tagName=="A") {
            var tr=target.parentNode.parentNode;
            var progressTd = tr.getElementsByTagName("TD")[2];
            var deadline =tr.getElementsByTagName("TD")[4];
            deadline.innerHTML = "";
            progressTd.innerHTML = "";
            var progress = document.createElement("progress");
            var span = document.createElement("span");
            progress.max = "100";
            progress.value = "0";
            progressTd.appendChild(progress);
            progressTd.appendChild(span);
            span.innerHTML="0";

            var file=tr.getElementsByTagName("td")[0].getElementsByTagName("input")[0].files[0];
            if(file.size>1024*1024*1024*4){alert("单个文件最大支持4g");return ;}
            var xhr=createXHR();
            var url="file/upload/fileSize="+file.size+"&fileName="+file.name;
            xhr.open("POST",url,true);
            xhr.setRequestHeader("Content-Type","multipart/form-data");//Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryi4ZATcKJNIfKPL4e
            xhr.responseType="text";
            xhr.onreadystatechange=function(){      //请求完成后的反馈
                if(xhr.readyState==4&&xhr.status==200){
                    if(xhr.responseText=="upload:success")alert("上传成功"); 
                    else if(xhr.responseText=="upload:error"){
                        alert("上传失败");
                    }
                    else window.location.replace("error.html");   //与后台交互不正常，跳转到错误页面
                }
            }
            xhr.upload.onprogress=function(ev){
                if(ev.lengthComputable){
                    var precent=Math.floor(100*ev.loaded/ev.total);
                    if(precent==100)precent=99;
                    progress.value=precent;
                    span.innerHTML=precent+"%";
                }
            }
            xhr.send(file);
            
        }
    }

    document.getElementById("uploadDetail").onclick=function(e){upload(e);}

    document.getElementById("accountLi").onclick=function(){
        window.location.replace("login.html");
    }

    document.getElementById("transferDetail").onclick=function(e){
        var target=e.target||window.event.target;
        if(target.tagName=="A") {
            var tr=target.parentNode.parentNode;
            if(target.innerHTML=="选择"){
                //0接收者账号 1昵称 2上传进度 3文件大小 4文件名称 5状态 6隐藏
                var file=document.createElement("input");
                file.type="file";
                file.accept="*/*";
                tr.getElementsByTagName("TD")[6].appendChild(file);
                targetTr=tr;
                file.onchange=function(){
                    tr.getElementsByTagName("TD")[3].innerHTML=file.files[0].size;
                    tr.getElementsByTagName("TD")[4].innerHTML=file.files[0].name;
                    target.innerHTML="开始";
                    var progress = document.createElement("progress");
                    progress.max = "100";
                    progress.value = "0";
                    tr.getElementsByTagName("TD")[2].appendChild(progress);
                    document.getElementById("transferDetail").appendChild(tr);
                }
                file.click();
            }
            else if(target.innerHTML=="开始"){
                if(account!=""){
                    var td=tr.getElementsByTagName("TD")[0];
                    var targetAccount=td.getElementsByTagName("input")[0].value;
                    var pattern1=/[^@]+@(\w+\.)+\w+/;  //邮箱格式
                    if(pattern1.test(targetAccount)){
                        var fileSize=tr.getElementsByTagName("TD")[3].innerHTML;
                        var fileName=tr.getElementsByTagName("TD")[4].innerHTML;
                        var message="command onlineRequest\n";
                        message+="srcaccount "+account+"\n";
                        message+="password "+password+"\n";
                        message+="targetAccount "+targetAccount+"\n";
                        message+="fileName "+fileName+"\n";
                        message+="fileSize "+fileSize+"\n\n";
                        ws.send(message);
                        //tr.getElementsByTagName("TD")[5].innerHTML="上传准备中";
                        target.innerHTML="准备中";
                        console.log(ws.bufferedAmount);
                    }
                    else {alert("请输入正确的邮箱格式");return;}
                }
                else {alert("在线传输需要您登录");return;}
            }
            else if(target.innerHTML=="准备中"||target.innerHTML=="发送"){
                targetTr.getElementsByTagName("TD")[5].getElementsByTagName("A")[0].innerHTML="选择";
            }
        }
    }

    document.getElementById("recordLi").onclick=function(){
        if(account!=""){
            var xhr=createXHR();
            xhr.open("POST","/getRecord",true);
            xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
            xhr.responseType="text";
            xhr.onreadystatechange=function(){
                if(xhr.readyState==4&&xhr.status==200){
                    var json = JSON.parse(xhr.responseText);
                    document.getElementById("recordDetail").innerHTML="";
                    var line;
                    for(var i=0;i<json.length;++i){
                        line="<tr>";
                        line+="<td>"+json[i]["operation"]+"</td>";
                        line+="<td>"+json[i]["fileName"]+"</td>";
                        line+="<td>"+json[i]["fileSize"]+"</td>";
                        line+="<td>"+json[i]["fileCode"]+"</td>";
                        line+="<td>"+json[i]["targetAccount"]+"</td>";
                        line+="<td>"+json[i]["nickname"]+"</td>";
                        line+="<td>"+json[i]["operateTime"]+"</td>";
                        line+="</tr>";
                        document.getElementById("recordDetail").innerHTML+=line;
                    }
                }
            }
            xhr.send(null);
        }
    }

    document.getElementById("openButton").onclick=function(){
        var fileCode= document.getElementById("fileCode").value;
        if(fileCode==null || fileCode.trim()=="")return alert("文件提取码为空！");
        else if(fileCode.trim().length!=20)return alert("文件提取码为20位英文字母加数字！");
        $("#openLi").click();
        var xhr=createXHR();
            xhr.open("POST","/getFile?fileCode="+fileCode,true);
            xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
            xhr.responseType="text";
            xhr.onreadystatechange=function(){
                if(xhr.readyState==4&&xhr.status==200){
                    document.getElementById("openTargetTd").innerHTML="";
                    document.getElementById("operateTd").innerHTML="";
                    var json = JSON.parse(xhr.responseText);
                    var index=json.fileName.lastIndexOf(".");
                    var fileKind="";
                    if(index!=-1)fileKind=json.fileName.substr(index+1);
                    document.getElementById("fileNameTd").innerHTML=json.fileName;
                    document.getElementById("fileSizeTd").innerHTML=json.fileSize;
                    document.getElementById("fileKindTd").innerHTML=fileKind;
                    document.getElementById("uploadTimeTd").innerHTML=json.uploadTime;
                    document.getElementById("openStatusTd").innerHTML="正常";
                    var timestamp =(new Date(json.uploadTime)).getTime();
                    document.getElementById("deadlineTd").innerHTML=stampToTime(timestamp+24*60*60*1000);
                    document.getElementById("fileCodeTd").innerHTML=json.fileCode;
                    document.getElementById("operateTd").innerHTML="<a href='/download?fileCode="+fileCode+"'>下载</a>";
                    if(fileKind=="mp4" || fileKind=="mp3"){
                        var video=document.createElement("video");
                        video.style="width:100%;height:460px";
                        video.controls="controls";
                        video.autoplay="autoplay";
                        video.src="/openOnline?fileCode="+fileCode;
                        document.getElementById("openTargetTd").appendChild(video);
                    }
                    else if(fileKind=="png" || fileKind=="jpg" || fileKind=="jpeg" || fileKind=="gif"){
                        var img=document.createElement("img");
                        img.style="width:100%;height:460px";
                        img.src="/openOnline?fileCode="+fileCode;
                        document.getElementById("openTargetTd").appendChild(img);
                    }
                    
                }
                else if(xhr.status==404){
                    document.getElementById("openStatusTd").innerHTML="失败";
                }


            }
        xhr.send(null);
    }
}


function setCookie(cname,cvalue,exdays){
    var d = new Date();
    d.setTime(d.getTime()+(exdays*24*60*60*1000));
    var expires = "expires="+d.toGMTString();
    document.cookie = cname+"="+cvalue+"; "+expires;
    
}
function getCookie(cname){
    var name = cname + "=";
    var ca = document.cookie.split(';');
    for(var i=0; i<ca.length; i++) {
        var c = ca[i].trim();
        if (c.indexOf(name)==0) { return c.substring(name.length,c.length); }
    }
    return "";
}

function logout(){
    document.cookie = "account=; password=; expires=Thu, 01 Jan 1970 00:00:00 GMT";
    window.location.replace("index.html");
}


function sendOnlineFile(fileKey,tr){
    /*var file=tr.getElementsByTagName("td")[6].getElementsByTagName("input")[0].files[0];
    var process=tr.getElementsByTagName("TD")[2].getElementsByTagName("progress")[0];
    var targetAccount=tr.getElementsByTagName("td")[0].getElementsByTagName("input")[0].value;
    if(file.size>10737418240){alert("文件的大小不能超过10g");return ;}
    var formData=new FormData();
    formData.append("file",file);
    $.ajax({
        url: "/sendOnlineFile?targetAccount="+targetAccount+"&fileSize="+file.size+"&fileKey="+fileKey,
        type: 'POST',
        cache: false,
        data: formData,
        dataType : "json",
        processData: false,
        contentType: false,
        async: true,
        xhr: function(){ 
            myXhr = $.ajaxSettings.xhr();
            if(myXhr.upload){ //检查upload属性是否存在
                //绑定progress事件的回调函数
                myXhr.upload.addEventListener('progress',function(ev){
                    if(ev.lengthComputable){
                        var precent=Math.floor(100*ev.loaded/ev.total);
                        if(precent==100)precent=99;
                        process.value=precent;
                    }
                }, false);
            }
            return myXhr; //xhr对象返回给jQuery使用
        },
        success: function(result){
            process.value=100;
            tr.getElementsByTagName("td")[5].innerHTML="完成";
            console.log("在线传输完成");
        },
        error:function(errmess){
            tr.getElementsByTagName("td")[5].innerHTML="失败";
            console.log("在线发送错误："+errmess);
        }
    });*/
    alert("在线传输尚未开通");
}

function addOnlineTr(){
    var tr=document.createElement("tr");
    tr.innerHTML=""+
    "<td><input type='email' placeholder='请输入对方邮箱'></td>"+
    "<td></td><td></td><td></td><td></td>"+
    "<td><a class='pay-now lg-layer'>选择</a></td>"+
    "<td style='display: none;'>神秘</td>";
    document.getElementById("transferDetail").appendChild(tr);

}



window.onload=function () {
    addEvent();
    var cookie=getCookie("account").trim();
    if(cookie!=""){
        document.getElementById("accountLi").onclick=function(){logout()};
        document.getElementById("accountText").innerHTML="退出系统";
        var temp=cookie.split("&");
        account=temp[0];
        password=(temp[1].split("="))[1];
        console.log("account:"+account);
        console.log("password:"+password);
        //connection();
    }
    
}

/*window.onbeforeunload = function(){
    return window.confirm("文件提取码即将丢失？大侠保存好了吗？");
}*/
