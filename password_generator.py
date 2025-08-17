import hashlib
import base64

def generatePassword(master_key: str, password: str, length: int = 24):
	return base64.urlsafe_b64encode(hashlib.pbkdf2_hmac("sha256", master_key.encode("utf-8"), password.encode("utf-8"), 100000)).decode("utf-8")[:length]