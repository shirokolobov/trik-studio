#pragma once

#include "bluetoothSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class BluetoothColorSensorImplementation : public BluetoothSensorImplementation
{
	Q_OBJECT

public:
	BluetoothColorSensorImplementation(RobotCommunicator *robotCommunicationInterface
			, inputPort::InputPortEnum const port
			, enums::lowLevelSensorType::SensorTypeEnum mode
			, sensorType::SensorTypeEnum const sensorType
		);
	virtual void read();
	void reconfigure(enums::lowLevelSensorType::SensorTypeEnum mode);

protected:
	virtual void sensorSpecificProcessResponse(QByteArray const &reading);
};

}
}
}
}
}
}
