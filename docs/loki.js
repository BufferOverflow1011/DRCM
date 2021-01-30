

var rootRef = firebase.database().ref().child("list");
rootRef.on("child_added", snap => {
    var name = snap.child("name").val();
    var address = snap.child("address").val();
    var email = snap.child("email").val();
    var mob_number = snap.child("mob_number").val();
    var clothing = snap.child("clothing").val();
    var food = snap.child("food").val();
    var med = snap.child("med").val();
    var power = snap.child("power").val();
   

    console.log(address)
    $("#content").append("<tr><td>" + name + "</td><td>" + address +"</td><td>" + email + "</td><td>" + mob_number + "</td><td>" + clothing + "</td><td>" + food + "</td><td>" + med + "</td><td>" + power + "</td></tr>")

    // alert(snap.val());;

});
