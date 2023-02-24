package de.farberg.traci.util;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.HashMap;
import java.util.Map;

import javax.xml.namespace.QName;
import javax.xml.stream.XMLEventReader;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.events.Attribute;
import javax.xml.stream.events.StartElement;
import javax.xml.stream.events.XMLEvent;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class EdgeIdToNameMapping {
	private Logger log = LoggerFactory.getLogger(EdgeIdToNameMapping.class);
	private File netFile;
	private Map<String, String> mapping = null;

	public EdgeIdToNameMapping(File netFile) throws FileNotFoundException, XMLStreamException {
		this.netFile = netFile;
		loadMapping();
	}

	public String getNameForEdgeId(String id) {
		return mapping.get(id);
	}

	private synchronized void loadMapping() throws FileNotFoundException, XMLStreamException {
		log.info("Loading edge id to name mapping from {}", netFile);
		mapping = new HashMap<>();

		FileReader fileReader = new FileReader(netFile);
		XMLInputFactory factory = XMLInputFactory.newInstance();
		XMLEventReader eventReader = factory.createXMLEventReader(fileReader);

		while (eventReader.hasNext()) {
			XMLEvent nextEvent = eventReader.nextEvent();
			if (!nextEvent.isStartElement())
				continue;

			if (nextEvent instanceof StartElement) {
				StartElement se = (StartElement) nextEvent;
				if ("edge".equals(se.getName().getLocalPart())) {
					Attribute idAttribute = se.getAttributeByName(new QName("id"));
					Attribute nameAttribute = se.getAttributeByName(new QName("name"));

					if (idAttribute != null && nameAttribute != null) {
						mapping.put(idAttribute.getValue(), nameAttribute.getValue());
					}

				}
			}

		}
	}

}
