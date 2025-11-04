var ws = new WebSocket("ws://localhost:9002");

const statusEl = document.getElementById("status");
statusEl.textContent = "Connecting...";

ws.onopen = function () {
    console.log("Connected to C++ WebSocket++ server");
    statusEl.textContent = "Connected!";
};

const canvas = document.getElementById("noiseCanvas");
const ctx = canvas.getContext("2d");

ws.onmessage = (event) => {
  try {
    const msg = JSON.parse(event.data);
    if (msg.width && msg.height && msg.data) {
      const imgData = ctx.createImageData(msg.width, msg.height);
      for (let i = 0; i < msg.data.length; i++) {
        const v = msg.data[i] * 255;
        const j = i * 4;
        imgData.data[j + 0] = v;    // R
        imgData.data[j + 1] = v;    // G
        imgData.data[j + 2] = v;    // B
        imgData.data[j + 3] = 255;  // A
      }
      ctx.putImageData(imgData, 0, 0);

      console.log("Received: Noise Image");
    } else {
      console.log("Received:", msg);
    }
  } catch (e) {
    console.error("Invalid message:", e);
  }
};

ws.onerror = function (err) {
    console.error("WebSocket error: ", err);
};

document.addEventListener("DOMContentLoaded", function () {
    var sendBtn = document.getElementById("sendBtn");
    if (sendBtn) {
        sendBtn.addEventListener("click", function () {
            if (ws.readyState === WebSocket.OPEN) {
                ws.send("whiteNoise");
            }
            else {
                console.warn("WebSocket not open yet");
            }
        });
    }
});
