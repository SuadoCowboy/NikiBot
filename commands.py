import discord
import math
import hashlib
import base64
from constants import *
import niki_process

def removeAtEveryone(message: str):
	return message.replace('@everyone', 'no u').replace('@here', 'nope')

def asCode(message: str):
	return f'```lua\n{message}```'

async def help_command(interaction: discord.Interaction, _args: list[str], isOwner: bool):
	if not interaction.app_permissions.send_messages and not isOwner:
		return

	out = ''
	for command in commands:
		out += command+'\n'

	await interaction.channel.send(content=asCode(out))

async def say_command(interaction: discord.Interaction, args: list[str], isOwner: bool):
	if not interaction.app_permissions.send_messages and not isOwner:
		return

	out = ''
	for arg in args:
		out += arg+' '

	await interaction.channel.send(content=removeAtEveryone(out))

async def math_command(interaction: discord.Interaction, args: list[str], isOwner: bool):
	if not interaction.app_permissions.send_messages and not isOwner:
		return

	if len(args) < 2:
		await interaction.channel.send(content='Usage: math v[consoleVariable] s[expression] s[variables...?]')
		return

	variables = {}
	for arg in args[2:]:
		arg = arg.split('=')
		for key in variables.keys():
			if key in arg[1]:
				arg[1] = arg[1].replace(key, variables[key])
		variables[arg[0]] = arg[1]

	expression = args[1]
	for variable in variables:
		expression = expression.replace(variable, variables[variable])

	cvarValue = eval(expression, {'math': math})

	niki_process.handleNikiCMDProcess(f'var {args[0]} {cvarValue}', 1 if isOwner else 0)
	await interaction.channel.send(content=asCode(cvarValue))

async def passwordgen_command(interaction: discord.Interaction, args: list[str], isOwner: bool):
	if not interaction.app_permissions.send_messages and not isOwner:
		return

	if len(args) != 2:
		await interaction.channel.send(content='Usage: passwordgen s[master_key] s[password]')
		return

	await interaction.channel.send(base64.urlsafe_b64encode(hashlib.pbkdf2_hmac("sha256", args[0].encode("utf-8"), args[1].encode("utf-8"), 100000)).decode("utf-8")[:32])

commands = {'help': help_command, 'say': say_command, 'math': math_command, 'passwordgen': passwordgen_command}