// Reference messages collection
var messagesRef = firebase.database().ref('contactformmessages');

$('#contactForm').submit(function(e) {
    e.preventDefault();

    var newMessageRef = messagesRef.push();
    newMessageRef.set({
        name: $('.fullname').val(),
        email: $('.email').val(),
        subject: $('.subject').val(),
        message: $('.message').val()
    });

    $('.success-message').show();

    $('#contactForm')[0].reset();
});
