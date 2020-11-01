inst = mc.mcGetInstance()

ModbusMPG = {}

ModbusMPG.mpgLastSelectedAxis = 0
ModbusMPG.lastMPGEncoderCounts = 0
ModbusMPG.lastMPGCountsMoved = 0
ModbusMPG.machEncoderCounts = 0
ModbusMPG.axisChanged = false
ModbusMPG.mpgSelectedAxis = -1

mc.mcMpgSetAccel(inst, 0, 80)  
mc.mcMpgSetRate(inst, 0, 100)
mc.mcMpgSetCountsPerDetent(inst, 0, 4)

local modbusStatusReg = mc.mcRegGetHandle(inst, "mbcntl/status")
local modbusRunningReg = mc.mcRegGetHandle(inst, "gRegs0/ModbusRunning")
local modbusFunc0ErrorReg = mc.mcRegGetHandle(inst, "modbus0/function0/rc") 
local modbusFunc1ErrorReg = mc.mcRegGetHandle(inst, "modbus0/function1/rc")

local modbusMPGEncoderCountsReg = mc.mcRegGetHandle(inst, "modbus0/MPGEnc")
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

function ModbusMPG.restartModbusConnection()
	local modbusCmdReg = mc.mcRegGetHandle(inst, "mbcntl/command")
	mc.mcRegSetValueString(modbusCmdReg, "STOP")
	mc.mcRegSetValueString(modbusCmdReg, "START")
end

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

	mc.mcMpgSetInc(inst, 0, incVal)
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
		ModbusMPG.axisChanged = true
		ModbusMPG.mpgLastSelectedAxis = ModbusMPG.mpgSelectedAxis
		ModbusMPG.lastMPGEncoderCounts = modbusMPGEncoderCounts
		ModbusMPG.lastMPGCountsMoved = modbusMPGEncoderCounts
		ModbusMPG.machEncoderCounts = 0
	end
end

-- a separate global variable is used to handle the 16-bit integer rollover of encoder
-- counts. this will continue the counts in the + or - direction "indefinitely" instead
-- of rolling over to max/min size of 16-bit integer once the encoder count extent is reached.
function processMPGMove(modbusMPGEncoderCounts)
	local currentMachMPGEncCounts = ModbusMPG.machEncoderCounts
	local newMachMPGEncCounts = currentMachMPGEncCounts + math.fmod((modbusMPGEncoderCounts - ModbusMPG.lastMPGEncoderCounts), 32700)
	
	if ModbusMPG.lastMPGEncoderCounts ~= modbusMPGEncoderCounts then
		ModbusMPG.machEncoderCounts = newMachMPGEncCounts

		-- there is a weird bug when switching axes while the other axis is still moving.
		-- instead of finishing the last move and starting a new move, something reverts the last
		-- axis to where it was before the MPG move and moves the new axis to the current encoder counts
		-- of the pendant. That is why the program is "forced" to only accept a delta in counts less than 100
		if ModbusMPG.axisChanged then
			ModbusMPG.axisChanged = false
			mc.mcMpgSetAxis(inst, 0, ModbusMPG.mpgSelectedAxis - 1)
		else
			if math.abs(newMachMPGEncCounts - ModbusMPG.lastMPGCountsMoved) < 100 then
				mc.mcMpgMoveCounts(inst, 0, newMachMPGEncCounts - ModbusMPG.lastMPGCountsMoved)
			end
		end
		
		ModbusMPG.lastMPGCountsMoved = newMachMPGEncCounts
		ModbusMPG.lastMPGEncoderCounts = newMachMPGEncCounts
	end
end

function ModbusMPG.RunModbusMPG() 
	local modbusRunning = mc.mcRegGetValueString(modbusStatusReg) == "RUNNING"
	modbusRunning = modbusRunning and mc.mcRegGetValue(modbusFunc0ErrorReg) == mc.MERROR_NOERROR
	modbusRunning = (modbusRunning and mc.mcRegGetValue(modbusFunc1ErrorReg) == mc.MERROR_NOERROR) and 1 or 0

	mc.mcSignalSetState(modbusStateReg, modbusRunning == 1)
	mc.mcRegSetValueLong(modbusRunningReg, modbusRunning)

	if modbusRunning ~= 1 then return end

	local mpgEnabled = mc.mcSignalGetState(mpgEnableReg)
	local modbusMPGEncoderCounts = mc.mcRegGetValue(modbusMPGEncoderCountsReg)

	getSelectedMPGAxis()
	setMPGIncrement()

	if tonumber(ModbusMPG.mpgSelectedAxis) > 0 and mpgEnabled == 1 then
		handleAxisSelectionChange(modbusMPGEncoderCounts)	
		processMPGMove(modbusMPGEncoderCounts)
	end
end

return ModbusMPG
