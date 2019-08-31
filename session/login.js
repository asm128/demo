function							qrn								(x)			{ x = (x << 13) ^ x; return ( x * (x * x * 15661 + 715827883)  + 1500450271); }
function							qdg								(str)		{
	var										x								= 0;
	var										i								= 0;
	for(i = 0; i < str.length; i++)
		x									+= qrn(str[i]);
	return x;
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
	inputb64Pass.value					= btoa(inputPass.value);
	formLogin.submit();
}
