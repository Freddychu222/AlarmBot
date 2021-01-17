
//ready data
let time0 = 1200;
let msg0 = "Hello there";
function Ready() {
  time0 = document.getElementById('timebox').value;
  msg0 = document.getElementById('msgbox').value;
}
//backgroundb
function backcall(){
  Ready();
  firebase.database().ref('time/'+time0).set({
    Time : time0,
    Message : msg0
  });

}
