import discord

async def say_command(interaction: discord.Interaction, command: str, args: list[str], isOwner: bool):
	if not interaction.app_permissions.send_messages or not isOwner:
		return

	out = ''
	for arg in args:
		out += arg+' '

	await interaction.channel.send(content=out.replace('@everyone', 'no u').replace('@here', 'nope'))

async def help_command(interaction: discord.Interaction, _command: str, _args: list[str], isOwner: bool):
	if not interaction.app_permissions.send_messages or not isOwner:
		return

	out = '```lua\n'
	for command in commands:
		out += command+'\n'
	out += '```'

	print(out)
	await interaction.channel.send(content=out)

commands = {'say': say_command, 'help': help_command}