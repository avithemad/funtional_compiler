const inputField = document.getElementById("user-input");
const outputText = document.getElementById("output-text");

inputField.addEventListener("input", () => {
  outputText.textContent = inputField.value;
});
