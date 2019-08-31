function							formOnClickSubmit				(signup) {
	var										inputCheckUser					= document.getElementById('user_used');
	var										inputCheckPass					= document.getElementById('pass_used');
	if (inputCheckUser.value === '0' || inputCheckPass.value === '0')
		return false;
	var										formLogin						= document.getElementById('login');
	var										inputUser						= document.getElementById('user');
	var										inputPass						= document.getElementById('pass');
	var										lowerUser						= inputUser.value.toLowerCase();
	if(signup) {
		var										inputTest						= document.getElementById('test');
		if(inputPass.value != inputTest.value) {
			inputTest.style.visibility = "visible";
			return false;
		}
	}
	var										inputb64User					= document.getElementById('b64user');
	var										inputb64Pass					= document.getElementById('b64pass');
	inputb64User.value						= btoa(lowerUser);
	inputb64Pass.value						= btoa(inputPass.value);
	formLogin.submit();
}
