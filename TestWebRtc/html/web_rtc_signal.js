var gstrUserId = "";
var gstrSdp = "";
var ws;

function StartSession()
{
  var txtUserId = document.getElementById('user_id');
  gstrUserId = txtUserId.value;

  if( gstrUserId.length == 0 )
  {
    alert( "user id is not defined" );
    return;
  }
  
  if( ws == null )
  {
  	ws = new WebSocket("ws://localhost:8080");
  	
  	// websocket 서버에 연결되면 연결 메시지를 화면에 출력한다.
		ws.onopen = function(e){
			ws.send( "req|register|" + gstrUserId );
		};

		// websocket 에서 수신한 메시지를 화면에 출력한다.
		ws.onmessage = function(e){
			var arrData = e.data.split("|");
			
			switch( arrData[0] )
			{
			case "res":
				switch( arrData[1] )
				{
				case "register":
					break;
				case "invite":
					if( arrData[2] == '200' )
					{
						setAnswer( arrData[3] );
	
	          btnCall.disabled = true;
	          btnAccept.disabled = true;
	          btnDecline.disabled = true;
	          btnBye.disabled = false;
          }
					break;
				}
				break;
			case "req":
				switch( arrData[1] )
				{
				case "invite":
					gstrToId = arrData[2];
          var txtPeerId = document.getElementById('peer_id');
          txtPeerId.value = gstrToId;

          Log("Invite event peer(" + gstrToId + ") sdp(" + arrData[3]+ ")" );
          //createAnswer( clsData.sdp );

          btnCall.disabled = true;
          btnAccept.disabled = false;
          btnDecline.disabled = false;
          btnBye.disabled = true;
					break;
				case "bye":
					gstrToId = "";
          stopPeer();
          break;
				}
			}
		};

		// websocket 세션이 종료되면 화면에 출력한다.
		ws.onclose = function(e){
			ws = null;
		}
  }
}

var gstrToId;

function Invite(strSdp)
{
	ws.send( "req|invite|" + gstrToId + "|" + strSdp );
}

/** 전화 수신한다. */
function Accept(strSdp)
{
	ws.send( "res|invite|200|" + strSdp );
}

var iLogMaxRowCount = 3000;
var iLogRowCount = 0;

function Log(strLog)
{
  var lyLog = document.getElementById('log');
  var clsDate = new Date();
  var strTime = "[" + clsDate.getHours() + ":" + clsDate.getMinutes() + ":" + clsDate.getSeconds() + "] ";

  if( iLogRowCount == iLogMaxRowCount )
  {
    lyLog.innerHTML = strTime + strLog + "<br>";
    iLogRowCount = 0;
  }
  else
  {
    lyLog.innerHTML += strTime + strLog + "<br>";
  }

  ++iLogRowCount;

  console.log(strLog);
}

function SendInvite()
{
  var txtPeerId = document.getElementById('peer_id');
  var strPeerId = txtPeerId.value;

  if( strPeerId.length == 0 )
  {
    alert( 'peer id is not defined' );
    return;
  }

  btnCall.disabled = true;
  btnAccept.disabled = true;
  btnDecline.disabled = true;
  btnBye.disabled = true;

  gstrToId = strPeerId;
  createOffer();

  //Invite( "o=" + strPeerId );
}

function SendAccept()
{
  btnCall.disabled = true;
  btnAccept.disabled = true;
  btnDecline.disabled = true;
  btnBye.disabled = true;

  createAnswer(gstrSdp);
  //Accept( "o=accept" );
}

/** INVITE 거절 응답을 전송한다. */
function SendDecline()
{
	ws.send( "res|invite|603" );
}

/** BYE 를 전송한다. */
function SendBye()
{
  stopPeer();
  ws.send( "req|bye" );
}

