#include "ProjectFile.h"

bool ProjectFile::save(
	const QString& a_path,
	const LibraryModel* const a_library_model,
	const PatternScene* a_pattern_scene)
{
	QFile file(a_path);
	if (!file.open(QIODevice::WriteOnly))
		return false;
	
	QXmlStreamWriter xml(&file);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	{
		xml.writeStartElement(ElementProject);
		xml.writeAttribute("href", "https://github.com/maxy-v/qjsynth");
		xml.writeAttribute("version", 0);
		{
			// Pattern properties
			xml.writeStartElement(ElementPatternSize);
			xml.writeAttribute(AttributeValue, QString::number(a_pattern_scene->size()) );
			xml.writeEndElement();

			// Instruments
			for (int i = 0; i < a_library_model->rowCount(); i++)
			{
				QModelIndex name_index   = a_library_model->index(i, LibraryModel::Column::Name);
				QModelIndex script_index = a_library_model->index(i, LibraryModel::Column::Script);
				QModelIndex color_index  = a_library_model->index(i, LibraryModel::Column::Color);
				xml.writeStartElement(ElementSynth);
				xml.writeAttribute(AttributeName  , name_index  .data()                  .value<QString>() );
				xml.writeAttribute(AttributeScript, script_index.data()                  .value<QString>() );
				xml.writeAttribute(AttributeColor , color_index .data(Qt::BackgroundRole).value<QColor >().name() );
				xml.writeEndElement();
			}

			// Pattern items
			auto items = a_pattern_scene->items();
			for (auto item: items)
			{
				int index = instrumentIndex(item, a_library_model);
				xml.writeStartElement(ElementPatternItem);// key, frame, size, instrument index
				xml.writeAttribute(AttributeKey            , QString::number( item->key()   ) );
				xml.writeAttribute(AttributeFrame          , QString::number( item->frame() ) );
				xml.writeAttribute(AttributeSize           , QString::number( item->size()  ) );
				xml.writeAttribute(AttributeInstrumentIndex, QString::number( index         ) );
				xml.writeEndElement();
			}
		}
		xml.writeEndElement();
	}
	xml.writeEndDocument();

	return true;
}

bool ProjectFile::load(
	const QString& a_path,
	LibraryModel* const a_library_model,
	PatternScene* a_pattern_scene
)
{
	QFile file(a_path);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	// Load data to temporary objects

	LibraryModel library_model;
	QList<PatternItemAttributes> pattern_items;
	int pattern_size = Properties::Pattern::DefaultSize;

	QXmlStreamReader xml(&file);

	while (!xml.atEnd())
	{
		QXmlStreamReader::TokenType type = xml.readNext();
		QStringRef                  name = xml.name();

		switch(type)
		{
		case QXmlStreamReader::Characters:
		case QXmlStreamReader::StartDocument:
		case QXmlStreamReader::EndDocument:
		case QXmlStreamReader::EndElement:
			break;
		case QXmlStreamReader::StartElement:
			{
				if      (name == ElementProject)
					;
				else if (name == ElementPatternSize)
				{
					SingleAttribute attribute = loadSingleAttribute(xml.attributes());
					if (!attribute.valid)
						return false;
					pattern_size = attribute.value.toInt();
					if (pattern_size < Properties::Pattern::MinSize)
						return false;
					if (pattern_size > Properties::Pattern::MaxSize)
						return false;
				}
				else if (name == ElementSynth)
				{
					InstrumentAttributes attributes = loadInstrumentAttributes(xml.attributes());
					if (!attributes.valid)
						return false;
					library_model.append(
						attributes.name,
						attributes.script,
						attributes.color
					);
				}
				else if (name == ElementPatternItem)
				{
					PatternItemAttributes attributes = loadPatternItemAttributes(xml.attributes(), &library_model);
					if (!attributes.valid)
						return false;
					pattern_items << attributes;
				}
				else
					return false;
			}
			break;
		case QXmlStreamReader::Invalid:
		default:
			return false;
		}
	}

	// Load data from temporary objects
	*a_library_model = std::move(library_model);
	a_pattern_scene->reset();
	for (auto& attributes: pattern_items)
	{
		PatternItem* item = new PatternItem(
			attributes.key,
			attributes.frame,
			attributes.size,
			a_library_model->item(attributes.instrument_index)
		);
		a_pattern_scene->addItem(item);
	}

	a_pattern_scene->setSize(pattern_size);

	return true;
}

int ProjectFile::instrumentIndex(
	const PatternItem* a_item,
	const LibraryModel* const a_library_model)
{
	QVector<int> library_items(a_library_model->rowCount());
	for (int i = 0; i < a_library_model->rowCount(); i++)
	{
		auto library_item = a_library_model->item(i);
		if (a_item->isSameType(library_item))
			return i;
	}
	Q_ASSERT( false && "Pattern item of unknown type" );
	return -1;
}

const ProjectFile::SingleAttribute ProjectFile::loadSingleAttribute(const QXmlStreamAttributes& a_attributes)
{
	SingleAttribute attributes;
	int count = 0;
	for (int i = 0; i < a_attributes.size(); i++)
	{
		auto& attribute = a_attributes.at(i);
		QStringRef name = attribute.name();
		
		count++;
		if      (name == AttributeValue)
			attributes.value = attribute.value().toString();
		else
			count--;
	}
	attributes.valid = (count==1);
	return attributes;
}

const ProjectFile::InstrumentAttributes ProjectFile::loadInstrumentAttributes(const QXmlStreamAttributes& a_attributes)
{
	InstrumentAttributes attributes;
	int count = 0;
	for (int i = 0; i < a_attributes.size(); i++)
	{
		auto& attribute = a_attributes.at(i);
		QStringRef name = attribute.name();
		
		count++;
		if      (name == AttributeName)
			attributes.name   = attribute.value().toString();
		else if (name == AttributeScript)
			attributes.script = attribute.value().toString();
		else if (name == AttributeColor)
			attributes.color = QColor(attribute.value());
		else
			count--;
	}
	attributes.valid = (count==3);
	return attributes;
}

const ProjectFile::PatternItemAttributes ProjectFile::loadPatternItemAttributes(
	const QXmlStreamAttributes& a_attributes,
	const LibraryModel* const a_library_model
)
{
	PatternItemAttributes attributes;
	int count = 0;
	for (int i = 0; i < a_attributes.size(); i++)
	{
		auto& attribute = a_attributes.at(i);
		QStringRef name = attribute.name();
		
		count++;
		if      (name == AttributeKey)
			attributes.key              = attribute.value().toInt();
		else if (name == AttributeFrame)
			attributes.frame            = attribute.value().toInt();
		else if (name == AttributeSize)
			attributes.size             = attribute.value().toInt();
		else if (name == AttributeInstrumentIndex)
			attributes.instrument_index = attribute.value().toInt();
		else
			count--;
	}
	attributes.valid =
		(count==4) &&
		attributes.size >= 1 &&
		(
			attributes.instrument_index >= 0 &&
			attributes.instrument_index < a_library_model->rowCount()
		);

	return attributes;
}
