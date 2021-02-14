

var rootRef = firebase.database().ref().child("list"); // lists -> parent
rootRef.on("child_added", snap => {
    var name = snap.child("name").val(); //childs
    var address = snap.child("address").val();
    var email = snap.child("email").val();
    var mob_number = snap.child("mob_number").val();
    var clothing = snap.child("clothing").val();
    var food = snap.child("food").val();
    var med = snap.child("med").val();
    var power = snap.child("power").val();
   var addr = snap.child("addr").val();

    console.log(address)
   
    $("#content").append("<tr><td>" + name + "</td><td>" + address +"</td><td>" + email + "</td><td>" + mob_number + "</td><td>" + clothing + "</td><td>" + food + "</td><td>" + med + "</td><td>" + power + "</td></tr>")
   
    
    // alert(snap.val());;

});

//geolocate
var tvx= firebase.database().ref().child("gps");
    tvx.on("child_added", snap => {
        var latlong = snap.child("latlong").val();
        var date = snap.child("date").val();
        var time = snap.child("time").val();
        var altitude = snap.child("altitude").val();
        console.log(latlong)
        var loc = "http://www.google.com/maps/place/"+latlong; 
        var divs = document.getElementById('sks');
        $("#gpx").html('<a href="' + loc + '"><lottie-player src="https://assets5.lottiefiles.com/packages/lf20_5TKRTc.json"  background="transparent"  speed="0.5"  style="width: 75px; height: 75px;" hover loop  autoplay></lottie-player></a>')

    });

