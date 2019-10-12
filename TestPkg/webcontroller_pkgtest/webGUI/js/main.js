$(function(){
    $("#table-btn").click(function(){
        $("#login-modal").fadeIn();
    });

    $("#submit-btn").click(function(){
        $("#login-modal").fadeOut();
    });

    $("#camstream").attr('data', 'http://' + location.hostname + ':10000/stream?topic=/cv_camera/image_raw');
    
    //http://172.20.10.3:10000/stream?topic=/image
    //http://172.20.10.3:10000/stream?topic=/cv_camera/image_raw

    var ros = new ROSLIB.Ros({url: "ws://" + location.hostname + ":9000"});

    ros.on("connection", function(){console.log("websocket: connected");});
    ros.on("error", function(error){console.log("websocket error", error);});
    ros.on("close", function(){console.log("websocket: closed");});

    var ls = new ROSLIB.Topic({
        ros : ros,
        name : 'lightsensor_val',
        messageType : 'raspi_ros/LightSensors',
    })

    ls.subscribe(message => {
        $(".value-right-side-modal").text(message.r_side);
        $(".value-right-front-modal").text(message.r_front);
        $(".value-left-front-modal").text(message.l_front);
        $(".value-left-side-modal").text(message.l_side);
        $(".value-sum-all-modal").text(message.sum);
        $(".value-sum-front-modal").text(message.sum_forward);

        $(".value-right-side").text(message.r_side);
        $(".value-right-front").text(message.r_front);
        $(".value-left-front").text(message.l_front);
        $(".value-left-side").text(message.l_side);
        $(".value-sum-all").text(message.sum);
        $(".value-sum-front").text(message.sum_forward);
        
        //console.log(message);
    });

    const motor_on = new ROSLIB.Service({
        ros : ros,
        name : "motor_on",
        serviceType:"std_srvs/Trigger",
    });

    const motor_off = new ROSLIB.Service({
        ros : ros,
        name : "motor_off",
        serviceType:"std_srvs/Trigger",
    });

    let request = new ROSLIB.ServiceRequest();

    $(".btn-gradient-3d-orange").click(function(){
        motor_on.callService(request, result => {
            console.log(result.success);
            console.log(result.message);
        });
    });

    $(".btn-gradient-3d-orange-green").click(function(){
        motor_off.callService(request, result => {
            console.log(result.success);
            console.log(result.message);
        });
    });

    const cmdVel = new ROSLIB.Topic({
        ros : ros,
        name : "motorCmdvel",
        messageType : "geometry_msgs/Twist"
    });

    const twist_front = new ROSLIB.Message({
        linear : {
            x : 0.125,
            y : 0.0,
            z : 0.0
           },
        angular : {
            x : 0.0,
            y : 0.0,
            z : 0.0
           }
    });

    $("#front").click(function(){
        cmdVel.publish(twist_front);
        console.log("Go front !");
    });

    const twist_turn_r = new ROSLIB.Message({
        linear : {
            x : 0.0,
            y : 0.0,
            z : 0.0
           },
        angular : {
            x : 0.0,
            y : 0.0,
            z : -1.57
           }
    });

    $("#right").click(function(){
        cmdVel.publish(twist_turn_r);
        console.log("Turn Right !");
    });

    const twist_turn_l = new ROSLIB.Message({
        linear : {
            x : 0.0,
            y : 0.0,
            z : 0.0
           },
        angular : {
            x : 0.0,
            y : 0.0,
            z : 1.57
           }
    });

    $("#left").click(function(){
        cmdVel.publish(twist_turn_l);
        console.log("Turn Left !");
    });

    const twist_back = new ROSLIB.Message({
        linear : {
            x : -0.125,
            y : 0.0,
            z : 0.0
           },
        angular : {
            x : 0.0,
            y : 0.0,
            z : 0.0
        }
    });

    $("#down").click(function(){
        cmdVel.publish(twist_back);
        console.log("Go back");
    });

    const stop = new ROSLIB.Message({
        linear : {
            x : 0.0,
            y : 0.0,
            z : 0.0
           },
        angular : {
            x : 0.0,
            y : 0.0,
            z : 0.0
           }
    });

    $(".btn-circle-stitch").click(function(){
        cmdVel.publish(stop);
    });
});