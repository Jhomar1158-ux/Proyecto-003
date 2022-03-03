
var mode = "/OFF";

function onStartStop(){
        var btn = document.querySelector("#startBtn");
        
        if (btn.value=="OFF") 
        {
            frm = 0;
            btn.value = "RUNNING...";
            mode = "/RUN";
        }
        else 
        {
            btn.value = "OFF";
            mode = "/OFF";
        }
        
        var url=mode;
        var xhr = new XMLHttpRequest();
        //xhr.onreadystatechange=ProcessResponse;
        xhr.open("GET",url, true);
        xhr.overrideMimeType('text/plain; charset=x-user-defined');
        xhr.send(null);
        xhr.onreadystatechange = function() {
            if (xhr.readyState == XMLHttpRequest.DONE && xhr.status == 200) {
                    console.log(xhr.readyState);
                }
        }
}

function pwmChange(){
        var pwm = document.querySelector("#pwm");
        var pwmValue = pwm.value;

        url = "/PWM?"+"val="+pwmValue.toString();

        var xhr = new XMLHttpRequest();
        //xhr.onreadystatechange=ProcessResponse;
        xhr.open("GET",url, true);
        xhr.overrideMimeType('text/plain; charset=x-user-defined');
        xhr.send(null);
        xhr.onreadystatechange = function() {
            if (xhr.readyState == XMLHttpRequest.DONE && xhr.status == 200) {
                    console.log(xhr.readyState);
                }
        }
}