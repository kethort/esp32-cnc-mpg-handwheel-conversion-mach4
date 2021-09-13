inst = mc.mcGetInstance()

ModbusMPG = {}

lastMPGEncoderCounts = 0
lastMPGCountsMoved = 0
machEncoderCounts = 0

ModbusMPG.mpgSelectedAxis = 0
ModbusMPG.mpgLastSelectedAxis = -1

ModbusMPG.mpgSelectedInc = 0
lastMPGInc = -1

mc.mcMpgSetAccel(inst, 11, 80)  
mc.mcMpgSetRate(inst, 11, 100)
mc.mcMpgSetCountsPerDetent(inst, 11, 4)

local modbusStatusReg = mc.mcRegGetHandle(inst, "mbcntl/status")
local modbusFunc0ErrorReg = mc.mcRegGetHandle(inst, "ModbusMPG/function0/rc") 
local modbusFunc1ErrorReg = mc.mcRegGetHandle(inst, "ModbusMPG/function1/rc")
local modbusFunc2ErrorReg = mc.mcRegGetHandle(inst, "ModbusMPG/function2/rc")

local modbusMPGEncoderCountsReg = mc.mcRegGetHandle(inst, "ModbusMPG/MPGEnc")

-- the MPG Handwheel will use software inputs 54-63 in the Mach4 Configuration
local xAxisSelReg = mc.mcSignalGetHandle(inst, mc.ISIG_INPUT54)
local yAxisSelReg = mc.mcSignalGetHandle(inst, mc.ISIG_INPUT55)
local zAxisSelReg = mc.mcSignalGetHandle(inst, mc.ISIG_INPUT56)
local aAxisSelReg = mc.mcSignalGetHandle(inst, mc.ISIG_INPUT57)
local bAxisSelReg = mc.mcSignalGetHandle(inst, mc.ISIG_INPUT58)
local cAxisSelReg = mc.mcSignalGetHandle(inst, mc.ISIG_INPUT59)
local mpgEnableReg = mc.mcSignalGetHandle(inst, mc.ISIG_INPUT60)
local mpgInc1SelReg = mc.mcSignalGetHandle(inst, mc.ISIG_INPUT61)
local mpgInc2SelReg = mc.mcSignalGetHandle(inst, mc.ISIG_INPUT62)
local mpgInc3SelReg = mc.mcSignalGetHandle(inst, mc.ISIG_INPUT63)
local modbusStateReg = mc.mcSignalGetHandle(inst, mc.OSIG_OUTPUT63)

function setMPGIncrement()
	local mpgInc1Selected = mc.mcSignalGetState(mpgInc1SelReg) == 1
	local mpgInc2Selected = mc.mcSignalGetState(mpgInc2SelReg) == 1
	local mpgInc3Selected = mc.mcSignalGetState(mpgInc3SelReg) == 1

	local incVal = 0

	if mpgInc1Selected then
		incVal = 0.001
	elseif mpgInc2Selected then
		incVal = 0.01
	elseif mpgInc3Selected then 
		incVal = 0.1
	end

	mc.mcMpgSetInc(inst, 11, incVal)
end

function getMPGIncrement()
	local mpgInc1Selected = mc.mcSignalGetState(mpgInc1SelReg) == 1
	local mpgInc2Selected = mc.mcSignalGetState(mpgInc2SelReg) == 1
	local mpgInc3Selected = mc.mcSignalGetState(mpgInc3SelReg) == 1

	if mpgInc1Selected then
		ModbusMPG.mpgSelectedInc = 1
	elseif mpgInc2Selected then
		ModbusMPG.mpgSelectedInc = 2
	elseif mpgInc3Selected then 
		ModbusMPG.mpgSelectedInc = 3
	end
end

function getSelectedMPGAxis()
	local xAxisSelected = mc.mcSignalGetState(xAxisSelReg)
	local yAxisSelected = mc.mcSignalGetState(yAxisSelReg)
	local zAxisSelected = mc.mcSignalGetState(zAxisSelReg)
	local aAxisSelected = mc.mcSignalGetState(aAxisSelReg)
	local bAxisSelected = mc.mcSignalGetState(bAxisSelReg)
	local cAxisSelected = mc.mcSignalGetState(cAxisSelReg)

	if xAxisSelected == 1 then
		ModbusMPG.mpgSelectedAxis = 1
	elseif yAxisSelected == 1 then
		ModbusMPG.mpgSelectedAxis = 2
	elseif zAxisSelected == 1 then
		ModbusMPG.mpgSelectedAxis = 3
	elseif aAxisSelected == 1 then
		ModbusMPG.mpgSelectedAxis = 4
	elseif bAxisSelected == 1 then
		ModbusMPG.mpgSelectedAxis = 5
	elseif cAxisSelected == 1 then
		ModbusMPG.mpgSelectedAxis = 6
	end
end

function handleAxisSelectionChange(modbusMPGEncoderCounts)
	if ModbusMPG.mpgLastSelectedAxis ~= ModbusMPG.mpgSelectedAxis then
		mc.mcMpgSetAxis(inst, 11, ModbusMPG.mpgSelectedAxis - 1)
		ModbusMPG.mpgLastSelectedAxis = ModbusMPG.mpgSelectedAxis
		lastMPGEncoderCounts = modbusMPGEncoderCounts
		lastMPGCountsMoved = modbusMPGEncoderCounts
		machEncoderCounts = 0
	end
end

-- a separate global variable is used to handle the 16-bit integer rollover of encoder
-- counts. this will continue the counts in the + or - direction "indefinitely" instead
-- of rolling over to max/min size of 16-bit integer once the encoder count extent is reached.
function processMPGMove(modbusMPGEncoderCounts)
	local currentMachMPGEncCounts = machEncoderCounts
	local newMachMPGEncCounts = currentMachMPGEncCounts + math.fmod((modbusMPGEncoderCounts - lastMPGEncoderCounts), 32700)

	if lastMPGEncoderCounts ~= modbusMPGEncoderCounts then
		machEncoderCounts = newMachMPGEncCounts

		-- there is a weird bug when switching axes while the other axis is still moving.
		-- instead of finishing the last move and starting a new move, something reverts the last
		-- axis to where it was before the MPG move and moves the new axis to the current encoder counts
		-- of the pendant. That is why the program is "forced" to only accept a delta in counts less than 100
		if math.abs(newMachMPGEncCounts - lastMPGCountsMoved) < 100 then
			mc.mcMpgSetAxis(inst, 11, ModbusMPG.mpgSelectedAxis - 1)
			mc.mcMpgMoveCounts(inst, 11, newMachMPGEncCounts - lastMPGCountsMoved)
		end

		
		lastMPGCountsMoved = newMachMPGEncCounts
		lastMPGEncoderCounts = newMachMPGEncCounts
	end
end

function modbusIsRunning()
	ModbusMPG.modbusRunning = mc.mcRegGetValueString(modbusStatusReg) == "RUNNING"
	ModbusMPG.modbusRunning = ModbusMPG.modbusRunning and mc.mcRegGetValue(modbusFunc0ErrorReg) == mc.MERROR_NOERROR
	ModbusMPG.modbusRunning = ModbusMPG.modbusRunning and mc.mcRegGetValue(modbusFunc1ErrorReg) == mc.MERROR_NOERROR
	ModbusMPG.modbusRunning = (ModbusMPG.modbusRunning and mc.mcRegGetValue(modbusFunc2ErrorReg) == mc.MERROR_NOERROR) and true or false
end

function ModbusMPG.restartModbusConnection()
	local modbusCmdReg = mc.mcRegGetHandle(inst, "mbcntl/command")
	mc.mcRegSetValueString(modbusCmdReg, "STOP")
	mc.mcRegSetValueString(modbusCmdReg, "START")
end

function ModbusMPG.RunModbusMPG() 
	modbusIsRunning()

	if not ModbusMPG.modbusRunning then return end

	local mpgEnabled = mc.mcSignalGetState(mpgEnableReg) ~= 0
	local modbusMPGEncoderCounts = mc.mcRegGetValue(modbusMPGEncoderCountsReg)

	getSelectedMPGAxis()
	getMPGIncrement()

	if lastMPGInc ~= ModbusMPG.mpgSelectedInc then
		setMPGIncrement()
		lastMPGInc = ModbusMPG.mpgSelectedInc
	end

	if tonumber(ModbusMPG.mpgSelectedAxis) > 0 and mpgEnabled then
		handleAxisSelectionChange(modbusMPGEncoderCounts)	
		processMPGMove(modbusMPGEncoderCounts)
	end
end

return ModbusMPG