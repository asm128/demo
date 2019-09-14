function							min								(a, b)		{ return (a < b) ? a : b; }
function							max								(a, b)		{ return (a > b) ? a : b; }
function							qrn								(x)			{ x = (x << 13) ^ x; return ( x * (x * x * 15661 + 715827883)  + 1500450271); }
function							qdg								(str)		{
	var										x								= 0;
	var										i								= 0;
	var 									j								= 0;
	var										filtered						= [];
	var										count							= max(1, str.length - 6);
	for(i = 0; i < count; i++) {
		for(j = 0; j < 6; j++)
			x									+= qrn(str[i + j]);
		x									+= x ^ (x << 11);
		filtered.push(x);
	}
	count								= max(filtered.length - 6, 1);
	for(i = 0; i < count; i++) {
		for(j = 0; j < 6; j++)
			filtered[i]							^= qrn(filtered[i + j]);
		filtered[i]							+= qrn(filtered[count - 1 - i]);
	}
	for(i = 1; i < count; i++) {
		for(j = 0; j < 6; j++) {
			filtered[j]							+= filtered[i + j];
			filtered[j]							+= filtered[filtered.length - 1 - i];
		}
	}
	var										result								= "";
	for(i = 0; i < min(filtered.length, 6); i++)
		result								+= filtered[i].toString();
	return result;
}

function							formOnClickSubmit				(signup)	{
	var										inputCheckUser					= document.getElementById('user_used');
	var										inputCheckPass					= document.getElementById('pass_used');
	if (inputCheckUser.value === '0' || inputCheckPass.value === '0')
		return false;
	var										formLogin						= document.getElementById('login');
	var										inputUser						= document.getElementById('user');
	var										inputPass						= document.getElementById('pass');
	if(signup) {
		var										inputTest						= document.getElementById('test');
		if(inputPass.value != inputTest.value) {
			var										inputTestRow					= document.getElementById('test_row');
			inputTestRow.style.visibility		= "visible";
			return false;
		}
	}
	var										inputb64User					= document.getElementById('b64user');
	var										inputb64Pass					= document.getElementById('b64pass');
	var										lowerUser						= inputUser.value.toLowerCase();
	inputb64User.value					= btoa(lowerUser);
	inputb64Pass.value					= btoa(qdg(inputPass.value));
	formLogin.submit();
}

function							validatePassword				(password) {
	if(password < 6)
		return false;
	return false;
}

function							validateUsername				(username) {
	if(username < 4)
		return false;
	if(username < 16)
		return false;
	return false;
}

function							validateSignUp					() {

	return false;
}

function							validateSignIn					() {
	return false;
}
