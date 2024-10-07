#include <iostream>
#include <fstream>

#include <E57SimpleReader.h>
#include <E57SimpleWriter.h>

void empty() {
	e57::Writer writer("empty.e57");
	if (!writer.Close()) throw std::string("Failed to close empty.e57");
}

void tiny_pc() {
	e57::Writer writer("tiny_pc.e57");
	e57::DateTime time;
	time.isAtomicClockReferenced = 1;
	time.dateTimeValue = 1.23;
	e57::Data3D header;
	header.guid = "guid";
	header.name = "name";
	header.description = "desc";
	header.sensorFirmwareVersion = "fw";
	header.sensorHardwareVersion = "hw";
	header.sensorSoftwareVersion = "sw";
	header.sensorModel = "model";
	header.sensorVendor = "vendor";
	header.sensorSerialNumber = "serial";
	header.relativeHumidity = 99;
	header.temperature = 20;
	header.acquisitionStart = time;
	header.acquisitionEnd = time;
	header.pointCount = 1;
	header.pointFields.cartesianXField = true;
	header.pointFields.cartesianYField = true;
	header.pointFields.cartesianZField = true;
	e57::Data3DPointsFloat buffers(header);
	buffers.cartesianX[0] = 1;
	buffers.cartesianY[0] = 2;
	buffers.cartesianZ[0] = 3;
	writer.WriteData3DData(header, buffers);
	if (!writer.Close()) throw std::string("Failed to close tiny_pc.e57");
}

void tiny_pc_with_extension() {
	e57::Writer writer("tiny_pc_with_extension.e57");
	e57::Data3D header;
	header.pointCount = 1;
	header.pointFields.cartesianXField = true;
	header.pointFields.cartesianYField = true;
	header.pointFields.cartesianZField = true;
	header.pointFields.pointRangeNodeType = e57::NumericalNodeType::Double;
	header.pointFields.normalXField = true;
	header.pointFields.normalYField = true;
	header.pointFields.normalZField = true;
	e57::Data3DPointsDouble buffers(header);
	buffers.cartesianX[0] = 1;
	buffers.cartesianY[0] = 2;
	buffers.cartesianZ[0] = 3;
	buffers.normalX[0] = 1;
	buffers.normalY[0] = 0;
	buffers.normalZ[0] = 0;
	writer.WriteData3DData(header, buffers);
	if (!writer.Close()) throw std::string("Failed to close tiny_pc_with_extension.e57");
}

void empty_pc() {
	e57::Writer writer("empty_pc.e57");
	e57::Data3D header;
	header.pointCount = 0;
	header.pointFields.cartesianXField = true;
	header.pointFields.cartesianYField = true;
	header.pointFields.cartesianZField = true;
	e57::Data3DPointsFloat buffers;
	buffers.cartesianX = new float[0];
	buffers.cartesianY = new float[0];
	buffers.cartesianZ = new float[0];
	writer.WriteData3DData(header, buffers);
	if (!writer.Close()) throw std::string("Failed to close empty_pc.e57");
}

void tiny_pc_and_images() {
	e57::Writer writer("tiny_pc_and_images.e57");

	e57::Data3D header;
	header.pointCount = 2;
	header.pointFields.cartesianXField = true;
	header.pointFields.cartesianYField = true;
	header.pointFields.cartesianZField = true;
	e57::Data3DPointsFloat buffers(header);
	buffers.cartesianX[0] = 0;
	buffers.cartesianY[0] = 0;
	buffers.cartesianZ[0] = 0;
	buffers.cartesianX[1] = 1;
	buffers.cartesianY[1] = 1;
	buffers.cartesianZ[1] = 1;
	writer.WriteData3DData(header, buffers);

	std::vector<uint8_t> jpegData;
	{
		std::ifstream ifs("../castle.jpg", std::ios::in | std::ios::binary);
		if (!ifs) throw std::string("Cannot open JPEG file");
		ifs.seekg(0, ifs.end);
		auto length = ifs.tellg();
		ifs.seekg(0, ifs.beg);
		jpegData.resize(length);
		ifs.read((char*)jpegData.data(), length);
	}

	std::vector<uint8_t> pngData;
	{
		std::ifstream ifs("../square.png", std::ios::in | std::ios::binary);
		if (!ifs) throw std::string("Cannot open PNG file");
		ifs.seekg(0, ifs.end);
		auto length = ifs.tellg();
		ifs.seekg(0, ifs.beg);
		pngData.resize(length);
		ifs.read((char*)pngData.data(), length);
	}

	e57::Image2D visImg;
	visImg.name = "visual";
	visImg.visualReferenceRepresentation.imageHeight = 100;
	visImg.visualReferenceRepresentation.imageWidth = 100;
	visImg.visualReferenceRepresentation.jpegImageSize = jpegData.size();
	writer.WriteImage2DData(visImg, e57::Image2DType::ImageJPEG, e57::Image2DProjection::ProjectionVisual, 0, jpegData.data(), jpegData.size());

	e57::Image2D sphImg;
	sphImg.name = "spherical";
	sphImg.sensorModel = "sensor";
	sphImg.sensorSerialNumber = "serial";
	sphImg.sensorVendor = "vendor";
	sphImg.associatedData3DGuid = header.guid;
	sphImg.description = "desc";
	sphImg.pose.rotation.x = 1;
	sphImg.pose.rotation.y = 0;
	sphImg.pose.rotation.z = 0;
	sphImg.pose.rotation.w = 0.5;
	sphImg.pose.translation.x = 1;
	sphImg.pose.translation.y = 2;
	sphImg.pose.translation.z = 3;
	sphImg.sphericalRepresentation.imageHeight = 100;
	sphImg.sphericalRepresentation.imageWidth = 100;
	sphImg.sphericalRepresentation.pixelHeight = 0.0314;
	sphImg.sphericalRepresentation.pixelWidth = 0.0314;
	sphImg.sphericalRepresentation.pngImageSize = pngData.size();
	writer.WriteImage2DData(sphImg, e57::Image2DType::ImagePNG, e57::Image2DProjection::ProjectionSpherical, 0, pngData.data(), pngData.size());

	e57::Image2D pinImg;
	pinImg.name = "pinhole";
	pinImg.pinholeRepresentation.imageHeight = 100;
	pinImg.pinholeRepresentation.imageWidth = 100;
	pinImg.pinholeRepresentation.pixelHeight = 0.033;
	pinImg.pinholeRepresentation.pixelWidth = 0.044;
	pinImg.pinholeRepresentation.focalLength = 123;
	pinImg.pinholeRepresentation.principalPointX = 23;
	pinImg.pinholeRepresentation.principalPointY = 42;
	pinImg.pinholeRepresentation.pngImageSize = pngData.size();
	writer.WriteImage2DData(pinImg, e57::Image2DType::ImageJPEG, e57::Image2DProjection::ProjectionPinhole, 0, jpegData.data(), jpegData.size());

	e57::Image2D cylImg;
	cylImg.name = "cylindrical";
	cylImg.cylindricalRepresentation.imageHeight = 100;
	cylImg.cylindricalRepresentation.imageWidth = 100;
	cylImg.cylindricalRepresentation.pixelHeight = 0.033;
	cylImg.cylindricalRepresentation.pixelWidth = 0.044;
	cylImg.cylindricalRepresentation.principalPointY = 42;
	cylImg.cylindricalRepresentation.radius = 666;
	cylImg.cylindricalRepresentation.pngImageSize = pngData.size();
	writer.WriteImage2DData(cylImg, e57::Image2DType::ImageJPEG, e57::Image2DProjection::ProjectionCylindrical, 0, jpegData.data(), jpegData.size());

	if (!writer.Close()) throw std::string("Failed to close tiny_pc_and_images.e57");
}

void tiny_spherical() {
	e57::Writer writer("tiny_spherical.e57");
	e57::Data3D header;
	header.pointCount = 360;
	header.pointFields.sphericalAzimuthField = true;
	header.pointFields.sphericalElevationField = true;
	header.pointFields.sphericalRangeField = true;
	header.pointFields.sphericalInvalidStateField = true;
	e57::Data3DPointsDouble buffers(header);
	for (auto i = 0; i < header.pointCount; i++) {
		buffers.sphericalAzimuth[i] = i * (3.14 / 360.0);
		buffers.sphericalElevation[i] = i * (3.14 / 360.0);
		buffers.sphericalRange[i] = 1.0;
		buffers.sphericalInvalidState[i] = i % 2 ? 1 : 0;
	}
	writer.WriteData3DData(header, buffers);
	if (!writer.Close()) throw std::string("Failed to close tiny_spherical.e57");
}

void original_guids() {
	e57::Writer writer("original_guids.e57");
	e57::Data3D header;
	header.pointCount = 1;
	header.pointFields.cartesianXField = true;
	header.pointFields.cartesianYField = true;
	header.pointFields.cartesianZField = true;
	header.originalGuids.push_back("guid1");
	header.originalGuids.push_back("guid2");
	header.originalGuids.push_back("guid3");
	e57::Data3DPointsFloat buffers(header);
	buffers.cartesianX[0] = 1;
	buffers.cartesianY[0] = 2;
	buffers.cartesianZ[0] = 3;
	writer.WriteData3DData(header, buffers);
}

void integer_intensity() {
	e57::Writer writer("integer_intensity.e57");
	e57::Data3D header;
	header.pointCount = 2;
	header.pointFields.cartesianInvalidStateField = true;
	header.pointFields.cartesianXField = true;
	header.pointFields.cartesianYField = true;
	header.pointFields.cartesianZField = true;
	header.pointFields.intensityField = true;
	header.pointFields.intensityNodeType = e57::NumericalNodeType::Integer;
	e57::Data3DPointsDouble buffers(header);
	buffers.cartesianInvalidState[0] = 0;
	buffers.cartesianX[0] = 1.1;
	buffers.cartesianY[0] = 2.2;
	buffers.cartesianZ[0] = 3.3;
	buffers.intensity[0] = -66;
	buffers.cartesianInvalidState[1] = 0;
	buffers.cartesianX[1] = 4.4;
	buffers.cartesianY[1] = 5.5;
	buffers.cartesianZ[1] = 6.6;
	buffers.intensity[1] = 66;
	writer.WriteData3DData(header, buffers);
	if (!writer.Close()) throw std::string("Failed to close integer_intensity.e57");
}

void scaled_integer_intensity() {
	e57::Writer writer("scaled_integer_intensity.e57");
	e57::Data3D header;
	header.pointCount = 2;
	header.pointFields.cartesianInvalidStateField = true;
	header.pointFields.cartesianXField = true;
	header.pointFields.cartesianYField = true;
	header.pointFields.cartesianZField = true;
	header.pointFields.intensityField = true;
	header.pointFields.intensityNodeType = e57::NumericalNodeType::ScaledInteger;
	header.pointFields.intensityScale = 0.1;
	e57::Data3DPointsDouble buffers(header);
	buffers.cartesianInvalidState[0] = 0;
	buffers.cartesianX[0] = 1.1;
	buffers.cartesianY[0] = 2.2;
	buffers.cartesianZ[0] = 3.3;
	buffers.intensity[0] = -66.6;
	buffers.cartesianInvalidState[1] = 0;
	buffers.cartesianX[1] = 4.4;
	buffers.cartesianY[1] = 5.5;
	buffers.cartesianZ[1] = 6.6;
	buffers.intensity[1] = 66.6;
	writer.WriteData3DData(header, buffers);
	if (!writer.Close()) throw std::string("Failed to close scaled_integer_intensity.e57");
}

// Read error reproduction case for
// https://github.com/cry-inc/e57/issues/12
void read_error() {
	e57::WriterOptions options;
	e57::Writer writer("read_error.e57", options);
	e57::Data3D header;
	header.pointCount = 2651;
	header.pointFields.colorRedField = true;
	header.pointFields.colorGreenField = true;
	header.pointFields.colorBlueField = true;
	header.pointFields.sphericalAzimuthField = true;
	header.pointFields.sphericalElevationField = true;
	header.pointFields.sphericalRangeField = true;
	header.pointFields.sphericalInvalidStateField = true;
	header.pointFields.intensityField = true;
	header.pointFields.rowIndexField = true;
	header.pointFields.columnIndexField = true;
	header.colorLimits.colorRedMaximum = 255;
	header.colorLimits.colorGreenMaximum = 255;
	header.colorLimits.colorBlueMaximum = 255;
	header.intensityLimits.intensityMinimum = 0;
	header.intensityLimits.intensityMaximum = 0;
	header.pointFields.pointRangeNodeType = e57::NumericalNodeType::Float;
	header.pointFields.angleNodeType = e57::NumericalNodeType::Float;
	header.pointFields.intensityNodeType = e57::NumericalNodeType::Integer;
	header.pointFields.columnIndexMaximum = 5742 - 1;
	header.pointFields.rowIndexMaximum = 8534 - 1;
	e57::Data3DPointsFloat buffer(header);
	for (size_t i = 0; i < header.pointCount; i++) {
		buffer.sphericalAzimuth[i] = 0;
		buffer.sphericalElevation[i] = 0;
		buffer.sphericalRange[i] = 0;
		buffer.sphericalInvalidState[i] = 0;
		buffer.colorRed[i] = 0;
		buffer.colorGreen[i] = 0;
		buffer.colorBlue[i] = 0;
		buffer.intensity[i] = 0;
		buffer.rowIndex[i] = 0;
		buffer.columnIndex[i] = 0;
	}
	writer.WriteData3DData(header, buffer);
	writer.Close();
}

int main() {
	empty();
	tiny_pc();
	tiny_pc_with_extension();
	empty_pc();
	tiny_pc_and_images();
	tiny_spherical();
	original_guids();
	integer_intensity();
	scaled_integer_intensity();
	read_error();

	std::cout << "Finished!\n";
}
