#include "inc/v3dexport.h"

#include "inc/billon.h"

#include <QXmlStreamWriter>
#include <QFile>
#include <QDebug>

namespace V3DExport
{
	namespace
	{
		void appendTags( QXmlStreamWriter &stream, const Billon &billon );
		void appendComponent( QXmlStreamWriter &stream, const Billon &billon, const int &index, const int &threshold );
		void appendPith( QXmlStreamWriter &stream, const Billon &billon );
		void writeXmlElementWithAttribute( QXmlStreamWriter &stream, const QString &elementName, const QString &attributeName, const QString &attributeValue, const QString &elementValue );
		void writeTag( QXmlStreamWriter &stream, const QString &name, const QString &value );
	}

	void process( const Billon &billon, const QString &fileName, const int &threshold )
	{
		QFile file(fileName);

		if( !file.open(QIODevice::WriteOnly) )
		{
			qDebug() << QObject::tr("ERREUR : Impossible de créer le ficher XML %1.").arg(fileName);
			return;
		}

		QXmlStreamWriter stream( &file );
		stream.setAutoFormatting(true);
		stream.writeStartDocument("1.0");
		stream.writeDTD("<!DOCTYPE IMAGE>");

			stream.writeStartElement("image");
				appendTags( stream, billon );
				appendComponent( stream, billon, 1, threshold );
				appendPith( stream, billon );
			stream.writeEndElement();

		stream.writeEndDocument();

		file.close();
	}

	namespace
	{
		void appendTags( QXmlStreamWriter &stream, const Billon &billon )
		{
			stream.writeStartElement("tags");
				writeTag(stream,"width",QString::number(billon.n_cols));
				writeTag(stream,"height",QString::number(billon.n_rows));
				writeTag(stream,"depth",QString::number(billon.n_slices));
				writeTag(stream,"xspacing",QString::number(billon.voxelWidth()));
				writeTag(stream,"yspacing",QString::number(billon.voxelHeight()));
				writeTag(stream,"zspacing",QString::number(billon.voxelDepth()));
				writeTag(stream,"voxelwidth",QString::number(billon.voxelWidth()));
				writeTag(stream,"voxelheight",QString::number(billon.voxelHeight()));
				writeTag(stream,"voxeldepth",QString::number(billon.voxelDepth()));
			stream.writeEndElement();
		}

		void appendComponent( QXmlStreamWriter &stream, const Billon &billon, const int &index, const int &threshold )
		{
			int width = billon.n_cols;
			int height = billon.n_rows;
			int depth = billon.n_slices;

			int i, j, k;

			stream.writeStartElement("components");
				stream.writeStartElement("component");
					stream.writeAttribute("id",QString::number(index));
					stream.writeAttribute("valmax",QString::number(billon.maxValue()));
					stream.writeAttribute("valmin",QString::number(billon.minValue()));

					//coord minimum
					stream.writeStartElement("coord");
						stream.writeAttribute("name","minimum");
						stream.writeStartElement("x");
							stream.writeCharacters(QString::number(0));
						stream.writeEndElement();
						stream.writeStartElement("y");
							stream.writeCharacters(QString::number(0));
						stream.writeEndElement();
						stream.writeStartElement("z");
							stream.writeCharacters(QString::number(0));
						stream.writeEndElement();
					stream.writeEndElement();

					//coord maximum
					stream.writeStartElement("coord");
						stream.writeAttribute("name","maximum");
						stream.writeStartElement("x");
							stream.writeCharacters(QString::number(width));
						stream.writeEndElement();
						stream.writeStartElement("y");
							stream.writeCharacters(QString::number(height));
						stream.writeEndElement();
						stream.writeStartElement("z");
							stream.writeCharacters(QString::number(depth));
						stream.writeEndElement();
					stream.writeEndElement();

					//binarydata
					stream.writeStartElement("binarydata");
						stream.writeAttribute("encoding","16");
						stream.writeCharacters("");
						QDataStream voxelStream(stream.device());
						for ( k=0; k<depth; ++k )
						{
							//data.clear();
							const Slice &slice = billon.slice(k);
							for ( j=0; j<height; ++j )
							{
								for ( i=0; i<width; ++i )
								{
									voxelStream << (qint16)(slice.at(j,i));
								}
							}
						}
						voxelStream.unsetDevice();
					stream.writeEndElement();

				stream.writeEndElement();
			stream.writeEndElement();
		}

		void appendPith( QXmlStreamWriter &stream, const Billon &billon )
		{
			if ( !billon.pith().isEmpty() )
			{
				const Pith &pith = billon.pith();
				const int depth = billon.n_slices;
				stream.writeStartElement("pith");
				for ( int k=0 ; k<depth ; ++k )
				{
					stream.writeStartElement("coord");
					stream.writeTextElement("x",QString::number(pith[k].x));
					stream.writeTextElement("y",QString::number(pith[k].y));
					stream.writeTextElement("z",QString::number(k));
					stream.writeEndElement();
				}
				stream.writeEndElement();
			}
		}

		void writeXmlElementWithAttribute( QXmlStreamWriter &stream, const QString &elementName, const QString &attributeName, const QString &attributeValue, const QString &elementValue )
		{
			stream.writeStartElement(elementName);
				stream.writeAttribute(attributeName,attributeValue);
				stream.writeCharacters(elementValue);
			stream.writeEndElement();
		}

		void writeTag( QXmlStreamWriter &stream, const QString &name, const QString &value )
		{
			writeXmlElementWithAttribute(stream,"tag","name",name,value);
		}
	}
}
