var ws = new WebSocket("ws://localhost:9002");

const statusEl = document.getElementById("status");
statusEl.textContent = "Connecting...";

ws.onopen = function () {
    console.log("Connected to C++ WebSocket++ server");
    statusEl.textContent = "Connected!";
};

ws.onmessage = function (event) {
    console.log("Message from server: ", event.data);
    var output = document.getElementById("output");
    if (output)
        output.textContent = event.data;
};

ws.onerror = function (err) {
    console.error("WebSocket error: ", err);
};

document.addEventListener("DOMContentLoaded", function () {
    var sendBtn = document.getElementById("sendBtn");
    if (sendBtn) {
        sendBtn.addEventListener("click", function () {
            if (ws.readyState === WebSocket.OPEN) {
                ws.send("Test Message from Front-End");
            }
            else {
                console.warn("WebSocket not open yet");
            }
        });
    }
});
