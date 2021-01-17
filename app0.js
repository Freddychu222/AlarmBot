//ready data
let time0 = 1200;
let msg0 = "Hello there";
function Ready() {
  time0 = document.getElementById('timebox').value;
}

function Ready2() {
  msg0 = document.getElementById('msgbox').value;
}
//backgroundb
function backcall(){
  Ready();
  firebase.database().ref('Time').set({
    Time : time0,
  });
}
function backcall2(){
  Ready2();
  firebase.database().ref('Message').set({
    Message : msg0
  });
}
