#!/usr/bin/env python3

# HTML page with CSS and JavaScript
print("""
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Calculator</title>
<style>
body {
    font-family: Arial, sans-serif;
    background-color: #f0f0f0;
    margin: 0;
    padding: 0;
    text-align: center;
}
.container {
    width: 300px;
    margin: 50px auto;
    background-color: #ffffff;
    padding: 20px;
    border-radius: 10px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
}
input[type="text"] {
    width: calc(100% - 20px);
    padding: 10px;
    margin-bottom: 10px;
    border: 1px solid #cccccc;
    border-radius: 5px;
}
select {
    width: calc(100% - 20px);
    padding: 10px;
    margin-bottom: 10px;
    border: 1px solid #cccccc;
    border-radius: 5px;
    background-color: #f9f9f9;
    cursor: pointer;
}
p {
    font-size: 18px;
}
</style>
<script>
function isNumeric(value) {
    return /^-?\d*[.,]?\d*$/.test(value);
}

function validateInput(input) {
    if (!isNumeric(input.value)) {
        input.value = input.value.replace(/[^0-9.,]/g, '');
    }
}

function calculate() {
    var num1 = parseFloat(document.getElementById('num1').value);
    var num2 = parseFloat(document.getElementById('num2').value);
    var operation = document.getElementById('operation').value;
    var result;
    if (!isNaN(num1) && !isNaN(num2)) {
        if (operation === '+') {
            result = num1 + num2;
        } else if (operation === '-') {
            result = num1 - num2;
        } else if (operation === '*') {
            result = num1 * num2;
        } else if (operation === '/') {
            if (num2 === 0) {
                result = 'Cannot divide by zero';
            } else {
                result = num1 / num2;
            }
        }
    } else {
        result = '';
    }
    document.getElementById('result').innerText = result;
}
</script>
</head>
<body>
<div class="container">
<h1>Calculator</h1>
<form oninput="calculate()">
<input type="text" id="num1" placeholder="Enter number" oninput="validateInput(this)">
<select id="operation">
<option value="+">+</option>
<option value="-">-</option>
<option value="*">*</option>
<option value="/">/</option>
</select>
<input type="text" id="num2" placeholder="Enter number" oninput="validateInput(this)">
</form>
<p>Result: <span id="result"></span></p>
</div>
</body>
</html>
""")
