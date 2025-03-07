import discord
from constants import *

async def help_command(interaction: discord.Interaction, _command: str, _args: list[str], isOwner: bool):
	if not interaction.app_permissions.send_messages and not isOwner:
		return

	out = '```lua\n'
	for command in commands:
		out += command+'\n'
	out += '```'

	await interaction.channel.send(content=out)

async def say_command(interaction: discord.Interaction, command: str, args: list[str], isOwner: bool):
	if not interaction.app_permissions.send_messages and not isOwner:
		return

	out = ''
	for arg in args:
		out += arg+' '

	await interaction.channel.send(content=out.replace('@everyone', 'no u').replace('@here', 'nope'))

async def math_command(interaction: discord.Interaction, _command: str, args: list[str], isOwner: bool):
	if not isOwner:
		return

	variables = {}
	if len(args) > 2:
		for arg in args[2:]:
			arg = arg.split('=')
			for key in variables.keys():
				if key in arg[1]:
					arg[1] = arg[1].replace(key, variables[key])
			variables[arg[0]] = arg[1]

	expression = args[1]
	for variable in variables:
		expression = expression.replace(variable, variables[variable])

print(eval(expression, {'math': math}))

commands = {'help': help_command, 'say': say_command, 'math': math_command}